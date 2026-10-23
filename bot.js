require('dotenv').config();
const TelegramBot = require('node-telegram-bot-api');
const path = require('path');
const fs = require('fs');

const token = process.env.BOT_TOKEN;

if (!token) {
  throw new Error('BOT_TOKEN is missing!');
}

const bot = new TelegramBot(token, { polling: true });

const PAGE_SIZE = 4;
const MAX_FILE_SIZE = 50 * 1024 * 1024; // حد تلغرام لرفع الملفات من البوتات
const SEND_DELAY_MS = 500; // تأخير بين الملفات لتفادي الخطأ 429

const semesters = {
  L1: {
    S1: {
      subjects: [
        'Analyse 1',
        'Algèbre 1',
        'Algorithmique Et Structure De Données 1',
        'Structure Machine 1',
        'Logiciels Libres (Open Source)',
        'Langue Etrangère',
        'Electricité Générale'
      ],
      folders: [
        'Analyse_1',
        'Algebre_1',
        'Algorithmique',
        'Structure_Machine',
        'Logiciels_Libres',
        'Langue_Etrangere',
        'Electricite_Generale'
      ]
    },

    S2: {
      subjects: [
        'Analyse 2',
        'Algèbre 2',
        'Algorithmique Et Structure De Données 2',
        'Structure Machine 2',
        'Logique Mathématique',
        "Introduction À L'Intelligence Artificielle",
        'Electronique Fondamentale'
      ],
      folders: [
        'Analyse_2',
        'Algebre_2',
        'Algorithmique_2',
        'Structure_Machine_2',
        'Logique_Mathematique',
        'Introduction_IA',
        'Electronique_Fondamentale'
      ]
    }
  },

  L2: {
    S3: {
      subjects: [
        'Architecture des ordinateurs',
        'Algorithmique et structure de données 3',
        "Systèmes d'information",
        'Programmation orientée objet 1',
        'Méthodes numériques',
        'Probabilités et statistique 1',
        'Gestion de projets'
      ],
      folders: [
        'Architecture_des_ordinateurs',
        'Algorithmique_3',
        'Systemes_d_information',
        'Programmation_orientee_objet_1',
        'Methodes_numeriques',
        'Probabilites_et_statistique_1',
        'Gestion_de_projets'
      ]
    },

    S4: {
      subjects: [
        "Système d'exploitation 1",
        'Théorie des langages',
        'Bases de données',
        'Programmation orientée objet 2',
        'Programmation Linéaire',
        'Probabilités et Statistique 2',
        'Entreprenariat'
      ],
      folders: [
        'Systeme_exploitation_1',
        'Theorie_des_langages',
        'Bases_de_donnees',
        'Programmation_orientee_objet_2',
        'Programmation_lineaire',
        'Probabilites_et_statistique_2',
        'Entreprenariat'
      ]
    }
  },

  L3: {
    S5: null,
    S6: null
  }
};

const contentTypes = [
  { id: 'exam', label: 'Examen final' },
  { id: 'rattrapage', label: 'Rattrapage' },
  { id: 'interro', label: 'Interrogations' },
  { id: 'summary', label: 'Résumés / Révision' },
  { id: 'solution', label: 'Solutions' },
  { id: 'code', label: 'Code' },
  { id: 'other', label: 'Autres fichiers' }
];

const supportedExtensions = new Set([
  '.pdf', '.jpg', '.jpeg', '.png', '.webp',
  '.doc', '.docx', '.ppt', '.pptx', '.txt',
  '.c', '.cpp', '.h', '.hpp', '.java', '.js',
  '.py', '.zip'
]);

const codeExtensions = new Set([
  '.c', '.cpp', '.h', '.hpp', '.java', '.js', '.py'
]);

function sleep(ms) {
  return new Promise((resolve) => setTimeout(resolve, ms));
}

function levelsKeyboard() {
  return [
    [
      { text: 'L1', callback_data: 'level_L1' },
      { text: 'L2', callback_data: 'level_L2' },
      { text: 'L3', callback_data: 'level_L3' }
    ]
  ];
}

// تنظيف النص: إزالة الحركات والتشكيل واستبدال الرموز بمسافات
function normalizeText(text) {
  return text
    .normalize('NFD')
    .replace(/[\u0300-\u036f]/g, '')
    .replace(/[_\-.]/g, ' ')
    .toLowerCase();
}

// يحدد نوع المحتوى من نص واحد (اسم ملف أو اسم مجلد)، أو null إذا لم يجد شيئًا
function detectType(text) {
  const t = normalizeText(text);

  if (/solution|corrig|correction/.test(t)) return 'solution';
  if (/interro|intero|quiz/.test(t)) return 'interro';
  if (/rattrapage|rettrapage/.test(t)) return 'rattrapage';
  if (/\b(exam|examen|examens|sujet|sujets)\b/.test(t)) return 'exam';
  if (/summary|summaries|revision|resume/.test(t)) return 'summary';
  if (/cours|course|lecture|chapter/.test(t)) return 'course';
  if (/\b(td|tp|exercise|exercice|tutorial)\b/.test(t)) return 'tdtp';

  return null;
}

// اسم المجلد: إذا جمع بين كلمات التصحيح وكلمات الامتحان معًا (مثل Exam_et_Solutions)
// فهو غامض، فنتركه ونعتمد على اسم الملف
function detectFolderType(folderName) {
  const t = normalizeText(folderName);

  const hasSolution = /solution|corrig|correction/.test(t);
  const hasExam = /\b(exam|examen|examens|sujet|sujets)\b|rattrapage|rettrapage|interro|intero|quiz/.test(t);

  if (hasSolution && hasExam) return null;

  return detectType(folderName);
}

// تصنيف ملفات مجلد library: تنظيم Drive هو المرجع، أي المجلدات من الأعمق إلى الأعم
// ثم اسم الملف إذا لم يدل أي مجلد على النوع
function classifyFile(fullPath, basePath) {
  const extension = path.extname(fullPath).toLowerCase();
  const fileName = path.basename(fullPath, extension);

  const folders = path
    .dirname(path.relative(basePath, fullPath))
    .split(path.sep)
    .filter((folder) => folder && folder !== '.')
    .reverse();

  for (const folder of folders) {
    const fromFolder = detectFolderType(folder);
    if (fromFolder) return fromFolder;
  }

  const fromName = detectType(fileName);
  if (fromName) return fromName;

  if (codeExtensions.has(extension)) return 'code';

  return 'other';
}

function getAvailableFiles(level, semester, index, contentTypeId) {
  const semesterData = semesters[level]?.[semester];

  if (!semesterData || !semesterData.folders[index]) {
    return [];
  }

  const folderName = semesterData.folders[index];

  const oldFolderPath = path.join(__dirname, 'pdf', level, folderName);
  const newFolderPath = path.join(__dirname, 'library', level, folderName);

  const results = [];
  const seen = new Set(); // لمنع تكرار نفس الملف إذا وُجد في المكتبتين

  function scanFolder(currentPath, basePath, forcedType) {
    if (!fs.existsSync(currentPath)) return;

    for (const entry of fs.readdirSync(currentPath, { withFileTypes: true })) {
      // تجاهل الملفات المخفية وملفات Office المؤقتة
      if (entry.name.startsWith('.') || entry.name.startsWith('~$')) continue;

      const fullPath = path.join(currentPath, entry.name);

      if (entry.isDirectory()) {
        scanFolder(fullPath, basePath, forcedType);
        continue;
      }

      if (!entry.isFile()) continue;

      const extension = path.extname(entry.name).toLowerCase();

      if (!supportedExtensions.has(extension)) continue;

      const type = forcedType || classifyFile(fullPath, basePath);

      if (type !== contentTypeId) continue;

      const stats = fs.statSync(fullPath);
      const fileName = path.basename(entry.name, extension);
      const dedupeKey = `${fileName.toLowerCase()}|${stats.size}`;

      if (seen.has(dedupeKey)) continue;
      seen.add(dedupeKey);

      // البحث عن السنة في المسار النسبي فقط حتى لا يتأثر بمسار جهازك
      const relativePath = path.relative(basePath, fullPath);
      const yearMatch = relativePath.match(/20\d{2}(?:[-_ ]20\d{2})?/);

      const is2026InterroImage =
  contentTypeId === 'interro' &&
  (
    entry.name === 'photo_5888542219319315287_y (1).jpg' ||
    entry.name === 'photo_5888542219319315285_y.jpg'
  );

results.push({
  name: fileName,
  type,
  year: is2026InterroImage
    ? '2026'
    : yearMatch
      ? yearMatch[0].replace(/_/g, '-')
      : 'Année inconnue',
  filePath: fullPath,
  size: stats.size,
  extension
});
    }
  }

  // المكتبة القديمة: pdf/<level>/<folder>/Examen_final أو Rattrapage
  if (contentTypeId === 'exam' || contentTypeId === 'rattrapage') {
    const oldExamFolder = path.join(
      oldFolderPath,
      contentTypeId === 'exam' ? 'Examen_final' : 'Rattrapage'
    );

    scanFolder(oldExamFolder, oldExamFolder, contentTypeId);
  }

  // المكتبة الجديدة: library/<level>/<folder>
  scanFolder(newFolderPath, newFolderPath, null);

  results.sort((a, b) =>
    a.name.localeCompare(b.name, undefined, { numeric: true })
  );

  return results;
}

// إرسال ملف واحد مع إعادة المحاولة عند الخطأ 429
async function sendFileSafely(chatId, file, caption) {
  for (let attempt = 0; attempt < 2; attempt++) {
    try {
      await bot.sendDocument(chatId, file.filePath, { caption });
      return true;
    } catch (err) {
      const retryAfter = err?.response?.body?.parameters?.retry_after;

      if (retryAfter && attempt === 0) {
        await sleep((retryAfter + 1) * 1000);
        continue;
      }

      console.error('Send error:', err.message);
      return false;
    }
  }

  return false;
}

function showSemesters(chatId, level) {
  const levelSemesters = semesters[level];

  if (!levelSemesters) {
    return bot.sendMessage(chatId, 'المستوى غير موجود.');
  }

  const keyboard = Object.keys(levelSemesters).map((semester) => [
    {
      text: `📚 ${semester}`,
      callback_data: `semester_${level}_${semester}`
    }
  ]);

  keyboard.push([
    {
      text: '⬅️ العودة إلى المستويات',
      callback_data: 'back_levels'
    }
  ]);

  return bot.sendMessage(chatId, `🎓 المستوى ${level}\n\nاختر السداسي:`, {
    reply_markup: { inline_keyboard: keyboard }
  });
}

function showSubjects(chatId, level, semester, page) {
  const semesterData = semesters[level]?.[semester];

  if (!semesterData) {
    return bot.sendMessage(
      chatId,
      `سيتم إضافة مقاييس ${level} ${semester} لاحقًا.`
    );
  }

  const subjects = semesterData.subjects;
  const totalPages = Math.ceil(subjects.length / PAGE_SIZE);
  const safePage = Math.min(Math.max(page, 0), totalPages - 1);

  const start = safePage * PAGE_SIZE;
  const pageSubjects = subjects.slice(start, start + PAGE_SIZE);

  const keyboard = pageSubjects.map((subject, index) => [
    {
      text: subject,
      callback_data: `subject_${level}_${semester}_${start + index}`
    }
  ]);

  const navigation = [];

  if (safePage > 0) {
    navigation.push({
      text: '⬅️ السابق',
      callback_data: `page_${level}_${semester}_${safePage - 1}`
    });
  }

  if (start + PAGE_SIZE < subjects.length) {
    navigation.push({
      text: 'التالي ➡️',
      callback_data: `page_${level}_${semester}_${safePage + 1}`
    });
  }

  if (navigation.length > 0) {
    keyboard.push(navigation);
  }

  keyboard.push([
    {
      text: '⬅️ العودة إلى الفصول',
      callback_data: `level_${level}`
    }
  ]);

  return bot.sendMessage(
    chatId,
    `📚 ${level} - ${semester}\n` +
      `الصفحة ${safePage + 1} من ${totalPages}\n\n` +
      'اختر المقياس:',
    { reply_markup: { inline_keyboard: keyboard } }
  );
}

bot.onText(/\/start/, (msg) => {
  bot.sendMessage(
    msg.chat.id,
    'مرحبًا بك في Student DZ 🎓\n\n' +
      'مكتبة الامتحانات والفروض الجامعية\n' +
      'جامعة برج بوعريريج\n\n' +
      'اختر مستواك:',
    { reply_markup: { inline_keyboard: levelsKeyboard() } }
  );
});

bot.on('callback_query', async (query) => {
  try {
    await bot.answerCallbackQuery(query.id);

    const chatId = query.message.chat.id;
    const choice = query.data;

    // العودة إلى قائمة المستويات
    if (choice === 'back_levels') {
      await bot.sendMessage(chatId, 'اختر مستواك:', {
        reply_markup: { inline_keyboard: levelsKeyboard() }
      });
      return;
    }

    // اختيار المستوى
    if (choice.startsWith('level_')) {
      const level = choice.slice('level_'.length);

      if (semesters[level]) {
        await showSemesters(chatId, level);
      }

      return;
    }

    // اختيار السداسي
    if (choice.startsWith('semester_')) {
      const [, level, semester] = choice.split('_');

      if (!semesters[level]?.[semester]) {
        await bot.sendMessage(chatId, 'سيتم إضافة مقاييس هذا الفصل لاحقًا.');
        return;
      }

      await showSubjects(chatId, level, semester, 0);
      return;
    }

    // التنقل بين صفحات المقاييس
    if (choice.startsWith('page_')) {
      const [, level, semester, pageText] = choice.split('_');
      await showSubjects(chatId, level, semester, Number(pageText));
      return;
    }

    // اختيار المقياس
    if (choice.startsWith('subject_')) {
      const [, level, semester, indexText] = choice.split('_');
      const index = Number(indexText);
      const subject = semesters[level]?.[semester]?.subjects[index];

      if (!subject) {
        await bot.sendMessage(chatId, 'لم أتمكن من العثور على هذا المقياس.');
        return;
      }

      const keyboard = [];

      for (let i = 0; i < contentTypes.length; i += 2) {
        keyboard.push(
          contentTypes.slice(i, i + 2).map((item) => ({
            text: item.label,
            callback_data: `content_${level}_${semester}_${index}_${item.id}`
          }))
        );
      }

      keyboard.push([
        {
          text: '⬅️ العودة إلى المقاييس',
          callback_data: `page_${level}_${semester}_0`
        }
      ]);

      await bot.sendMessage(chatId, `📘 ${subject}\n\nاختر تصنيف الملفات:`, {
        reply_markup: { inline_keyboard: keyboard }
      });

      return;
    }

    // اختيار تصنيف الملفات
    if (choice.startsWith('content_')) {
      const [, level, semester, indexText, contentTypeId] = choice.split('_');
      const index = Number(indexText);

      const subject = semesters[level]?.[semester]?.subjects[index];
      const selectedType = contentTypes.find((item) => item.id === contentTypeId);

      if (!subject || !selectedType) {
        await bot.sendMessage(chatId, 'حدث خطأ في اختيار التصنيف.');
        return;
      }

      const backButton = {
        reply_markup: {
          inline_keyboard: [
            [
              {
                text: '⬅️ العودة إلى التصنيفات',
                callback_data: `subject_${level}_${semester}_${index}`
              }
            ]
          ]
        }
      };

      const files = getAvailableFiles(level, semester, index, contentTypeId);

      if (files.length === 0) {
        await bot.sendMessage(
          chatId,
          `📘 ${subject}\n\nلا توجد ملفات ضمن تصنيف «${selectedType.label}» حاليًا.`,
          backButton
        );
        return;
      }

      for (const file of files) {
        if (!fs.existsSync(file.filePath)) {
          console.error('File not found:', file.filePath);
          continue;
        }

        if (file.size > MAX_FILE_SIZE) {
          await bot.sendMessage(
            chatId,
            `⚠️ الملف كبير جدًا ولا يمكن إرساله: ${file.name}`
          );
          continue;
        }

        const caption =
          `📘 ${subject}\n` +
          `📄 ${file.name}\n` +
          `النوع: ${selectedType.label}\n` +
          `السنة: ${file.year}`;

        const sent = await sendFileSafely(chatId, file, caption);

        if (!sent) {
          await bot.sendMessage(chatId, `⚠️ تعذر إرسال الملف: ${file.name}`);
        }

        await sleep(SEND_DELAY_MS);
      }

      await bot.sendMessage(chatId, 'يمكنك اختيار تصنيف آخر.', backButton);
      return;
    }
  } catch (err) {
    console.error('Callback error:', err);
  }
});

process.on('unhandledRejection', (err) => {
  console.error('Unhandled rejection:', err);
});

bot.on('polling_error', (err) => {
  console.error('Polling error:', err.message);
});

console.log('Student DZ is running!');

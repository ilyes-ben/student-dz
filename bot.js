
require('dotenv').config();

const TelegramBot = require('node-telegram-bot-api');
const path = require('path');
const fs = require('fs');

// ==========================================
// 1. إعداد البوت
// ==========================================

const token = process.env.BOT_TOKEN;

if (!token) {
    throw new Error('BOT_TOKEN is missing!');
}

const bot = new TelegramBot(token, { polling: true });

const REQUIRED_CHANNEL = '@StudentDZ_Official';

const verifiedUsers = new Set();

// ==========================================
// 2. نظام الاشتراك الإجباري
// ==========================================

async function isSubscribed(userId) {
    if (verifiedUsers.has(String(userId))) {
        return true;
    }

    try {
        const member = await bot.getChatMember(
            REQUIRED_CHANNEL,
            userId
        );

        const subscribed =
            member.status === 'creator' ||
            member.status === 'administrator' ||
            member.status === 'member' ||
            (
                member.status === 'restricted' &&
                member.is_member === true
            );

        if (subscribed) {
            verifiedUsers.add(String(userId));
        }

        return subscribed;

    } catch (error) {
        console.error('Subscription check error:', error.message);
        return null;
    }
}

async function sendSubscriptionPrompt(chatId) {
    await bot.sendMessage(
        chatId,
        '🔒 لاستخدام Student DZ، يجب الاشتراك في قناتنا أولًا.',
        {
            reply_markup: {
                inline_keyboard: [
                    [
                        {
                            text: '📢 الاشتراك في القناة',
                            url: 'https://t.me/StudentDZ_Official'
                        }
                    ],
                    [
                        {
                            text: '✅ تحققت من الاشتراك',
                            callback_data: 'check_subscription'
                        }
                    ]
                ]
            }
        }
    );
}

// ==========================================
// 3. نظام الإحصائيات
// ==========================================

const statsPath = path.join(__dirname, 'stats.json');

function loadStats() {
    try {
        if (fs.existsSync(statsPath)) {
            const data = JSON.parse(
                fs.readFileSync(statsPath, 'utf8')
            );

            if (!data.users || typeof data.users !== 'object') {
                data.users = {};
            }

            if (!data.subjects || typeof data.subjects !== 'object') {
                data.subjects = {};
            }

            if (typeof data.filesSent !== 'number') {
                data.filesSent = 0;
            }

            for (const user of Object.values(data.users)) {
                if (typeof user.starts !== 'number') {
                    user.starts = 0;
                }

                if (!user.firstSeen) {
                    user.firstSeen =
                        user.lastSeen || new Date().toISOString();
                }
            }

            return data;
        }

    } catch (error) {
        console.error('Could not read stats:', error.message);
    }

    return {
        users: {},
        subjects: {},
        filesSent: 0
    };
}

let stats = loadStats();

function saveStats() {
    try {
        fs.writeFileSync(
            statsPath,
            JSON.stringify(stats, null, 2)
        );

    } catch (error) {
        console.error('Could not save stats:', error.message);
    }
}

function recordActivity(from) {
    if (!from || from.is_bot) return;

    const userId = String(from.id);
    const now = new Date().toISOString();

    if (!stats.users[userId]) {
        stats.users[userId] = {
            firstSeen: now,
            lastSeen: now,
            starts: 0
        };
    }

    const user = stats.users[userId];

    user.lastSeen = now;
    user.first_name = from.first_name || user.first_name || '';
    user.last_name = from.last_name || user.last_name || '';
    user.username = from.username || user.username || '';

    saveStats();
}

function recordUser(msg) {
    if (!msg.from || msg.from.is_bot) return;

    const userId = String(msg.from.id);

    if (!stats.users[userId]) {
        const now = new Date().toISOString();

        stats.users[userId] = {
            firstSeen: now,
            lastSeen: now,
            starts: 0
        };
    }

    stats.users[userId].starts += 1;

    recordActivity(msg.from);
}

function formatDate(date) {
    if (!date) return 'غير معروف';

    const timestamp = new Date(date).getTime();

    if (Number.isNaN(timestamp)) {
        return 'غير معروف';
    }

    return new Date(timestamp).toLocaleString('ar-DZ', {
        dateStyle: 'short',
        timeStyle: 'short'
    });
}

// ==========================================
// 4. إعدادات المكتبة
// ==========================================

const PAGE_SIZE = 4;

const MAX_FILE_SIZE = 50 * 1024 * 1024;

// تأخير قصير بين الملفات
const SEND_DELAY_MS = 100;

// مدة تخزين نتائج البحث عن الملفات
const FILE_CACHE_DURATION = 60 * 1000;

// التخزين المؤقت
const fileCache = new Map();

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
                'Systèmes d information',
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
    '.pdf',
    '.jpg',
    '.jpeg',
    '.png',
    '.webp',
    '.doc',
    '.docx',
    '.ppt',
    '.pptx',
    '.txt',
    '.c',
    '.cpp',
    '.h',
    '.hpp',
    '.java',
    '.js',
    '.py',
    '.zip'
]);

const codeExtensions = new Set([
    '.c',
    '.cpp',
    '.h',
    '.hpp',
    '.java',
    '.js',
    '.py'
]);

function sleep(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
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

// ==========================================
// 5. تصنيف الملفات
// ==========================================

function normalizeText(text) {
    return text
        .normalize('NFD')
        .replace(/[\u0300-\u036f]/g, '')
        .replace(/[_\-.]/g, ' ')
        .toLowerCase();
}

function detectType(text) {
    const t = normalizeText(text);

    if (/\bcode\b/.test(t)) return 'code';

    if (/solution|corrig|correction/.test(t)) {
        return 'solution';
    }

    if (/interro|intero|quiz/.test(t)) {
        return 'interro';
    }

    if (/rattrapage|rettrapage/.test(t)) {
        return 'rattrapage';
    }

    if (/\b(exam|examen|examens|sujet|sujets)\b/.test(t)) {
        return 'exam';
    }

    if (/summary|summaries|revision|resume/.test(t)) {
        return 'summary';
    }

    if (/cours|course|lecture|chapter/.test(t)) {
        return 'course';
    }

    if (/\b(td|tp|exercise|exercice|tutorial)\b/.test(t)) {
        return 'tdtp';
    }

    return null;
}

function detectFolderType(folderName) {
    const t = normalizeText(folderName);

    const hasSolution =
        /solution|corrig|correction/.test(t);

    const hasExam =
        /\b(exam|examen|examens|sujet|sujets)\b|rattrapage|rettrapage|interro|intero|quiz/.test(t);

    if (hasSolution && hasExam) {
        return null;
    }

    return detectType(folderName);
}

function classifyFile(fullPath, basePath) {
    const extension = path.extname(fullPath).toLowerCase();

    const fileName = path.basename(fullPath, extension);

    const folders = path
        .dirname(path.relative(basePath, fullPath))
        .split(path.sep)
        .filter(folder => folder && folder !== '.')
        .reverse();

    for (const folder of folders) {
        const fromFolder = detectFolderType(folder);

        if (fromFolder) {
            return fromFolder;
        }
    }

    const fromName = detectType(fileName);

    if (fromName) {
        return fromName;
    }

    if (codeExtensions.has(extension)) {
        return 'code';
    }

    return 'other';
}

// ==========================================
// 6. البحث عن الملفات مع Cache
// ==========================================

function getAvailableFiles(level, semester, index, contentTypeId) {

    const cacheKey = `${level}_${semester}_${index}_${contentTypeId}`;

    const cached = fileCache.get(cacheKey);

    // استعمال النتائج المخزنة إذا لم تنتهِ مدتها
    if (
        cached &&
        Date.now() - cached.timestamp < FILE_CACHE_DURATION
    ) {
        return cached.files;
    }

    const semesterData = semesters[level]?.[semester];

    if (!semesterData || !semesterData.folders[index]) {
        return [];
    }

    const isPOO1 =
        level === 'L2' &&
        semester === 'S3' &&
        semesterData.subjects[index] === 'Programmation orientée objet 1';

    const folderName = isPOO1
        ? 'Programmation_orientee_objet_2'
        : semesterData.folders[index];

    const folderLevel = isPOO1 ? 'L2' : level;

    const oldFolderPath = path.join(
        __dirname,
        'pdf',
        folderLevel,
        folderName
    );

    const newFolderPath = path.join(
        __dirname,
        'library',
        folderLevel,
        folderName
    );

    const results = [];

    const seen = new Set();

    function scanFolder(currentPath, basePath, forcedType) {

        if (!fs.existsSync(currentPath)) {
            return;
        }

        let entries;

        try {
            entries = fs.readdirSync(currentPath, {
                withFileTypes: true
            });
        } catch (error) {
            console.error('Folder scan error:', error.message);
            return;
        }

        for (const entry of entries) {

            if (
                entry.name.startsWith('.') ||
                entry.name.startsWith('~$')
            ) {
                continue;
            }

            const fullPath = path.join(
                currentPath,
                entry.name
            );

            if (entry.isDirectory()) {

                let childForcedType = forcedType;

                if (!childForcedType) {
                    if (entry.name === 'Examen_final') {
                        childForcedType = 'exam';
                    } else if (entry.name === 'Rattrapage') {
                        childForcedType = 'rattrapage';
                    }
                }

                scanFolder(
                    fullPath,
                    basePath,
                    childForcedType
                );

                continue;
            }

            if (!entry.isFile()) {
                continue;
            }

            const extension = path.extname(entry.name).toLowerCase();

            if (!supportedExtensions.has(extension)) {
                continue;
            }

            const type =
                forcedType || classifyFile(fullPath, basePath);

            if (type !== contentTypeId) {
                continue;
            }

            let fileStats;

            try {
                fileStats = fs.statSync(fullPath);
            } catch (error) {
                continue;
            }

            const fileName = path.basename(
                entry.name,
                extension
            );

            const dedupeKey =
                `${fileName.toLowerCase()}|${fileStats.size}`;

            if (seen.has(dedupeKey)) {
                continue;
            }

            seen.add(dedupeKey);

            const relativePath = path.relative(
                basePath,
                fullPath
            );

            const yearMatch = relativePath.match(
                /20\d{2}(?:[-_ ]20\d{2})?/
            );

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
                size: fileStats.size,
                extension
            });
        }
    }

    scanFolder(oldFolderPath, oldFolderPath, null);

    scanFolder(newFolderPath, newFolderPath, null);

    results.sort((a, b) =>
        a.name.localeCompare(
            b.name,
            undefined,
            { numeric: true }
        )
    );

    // حفظ النتائج في الذاكرة المؤقتة
    fileCache.set(cacheKey, {
        files: results,
        timestamp: Date.now()
    });

    return results;
}

// ==========================================
// 7. إرسال الملفات بطريقة محسنة
// ==========================================

async function sendFileSafely(chatId, file, caption) {

    const maxAttempts = 2;

    for (let attempt = 1; attempt <= maxAttempts; attempt++) {

        try {

            await bot.sendDocument(
                chatId,
                file.filePath,
                {
                    caption: caption
                }
            );

            return true;

        } catch (error) {

            const statusCode =
                error?.response?.statusCode ||
                error?.response?.body?.error_code;

            const retryAfter =
                error?.response?.body?.parameters?.retry_after;

            const isTemporaryError = [
                500,
                502,
                503,
                504
            ].includes(statusCode);

            const isNetworkError = [
                'ECONNRESET',
                'ETIMEDOUT',
                'EAI_AGAIN',
                'ECONNREFUSED'
            ].includes(error.code);

            // إعادة المحاولة عند خطأ مؤقت
            if (
                attempt < maxAttempts &&
                (retryAfter || isTemporaryError || isNetworkError)
            ) {

                let delay = retryAfter
                    ? (retryAfter + 1) * 1000
                    : 500;

                console.log(
                    `Retrying file: ${file.name}`
                );

                await sleep(delay);

                continue;
            }

            console.error(
                `Send error for ${file.name}:`,
                error.message
            );

            return false;
        }
    }

    return false;
}

// ==========================================
// 8. القوائم
// ==========================================

function showSemesters(chatId, level) {

    const levelSemesters = semesters[level];

    if (!levelSemesters) {
        return bot.sendMessage(
            chatId,
            'المستوى غير موجود.'
        );
    }

    const keyboard = Object.keys(levelSemesters).map(
        semester => [
            {
                text: `📚 ${semester}`,
                callback_data: `semester_${level}_${semester}`
            }
        ]
    );

    keyboard.push([
        {
            text: '⬅️ العودة إلى المستويات',
            callback_data: 'back_levels'
        }
    ]);

    return bot.sendMessage(
        chatId,
        `🎓 المستوى ${level}\n\nاختر السداسي:`,
        {
            reply_markup: {
                inline_keyboard: keyboard
            }
        }
    );
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

    const totalPages = Math.ceil(
        subjects.length / PAGE_SIZE
    );

    const safePage = Math.min(
        Math.max(page, 0),
        totalPages - 1
    );

    const start = safePage * PAGE_SIZE;

    const pageSubjects = subjects.slice(
        start,
        start + PAGE_SIZE
    );

    const keyboard = pageSubjects.map(
        (subject, index) => [
            {
                text: subject,
                callback_data:
                    `subject_${level}_${semester}_${start + index}`
            }
        ]
    );

    const navigation = [];

    if (safePage > 0) {
        navigation.push({
            text: '⬅️ السابق',
            callback_data:
                `page_${level}_${semester}_${safePage - 1}`
        });
    }

    if (start + PAGE_SIZE < subjects.length) {
        navigation.push({
            text: 'التالي ➡️',
            callback_data:
                `page_${level}_${semester}_${safePage + 1}`
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
        {
            reply_markup: {
                inline_keyboard: keyboard
            }
        }
    );
}

// ==========================================
// 9. أمر البداية
// ==========================================

bot.onText(/^\/start(?:@\w+)?$/, async (msg) => {

    try {

        const subscription = await isSubscribed(
            msg.from.id
        );

        if (subscription === null) {

            await bot.sendMessage(
                msg.chat.id,
                'تعذّر التحقق من الاشتراك حاليًا. حاول مجددًا بعد قليل.'
            );

            return;
        }

        if (!subscription) {

            await sendSubscriptionPrompt(
                msg.chat.id
            );

            return;
        }

        recordUser(msg);

        await bot.sendMessage(
            msg.chat.id,

            'مرحبًا بك في Student DZ 🎓\n\n' +
            'مكتبة الامتحانات والفروض الجامعية\n' +
            'جامعة برج بوعريريج\n\n' +
            'اختر مستواك:',

            {
                reply_markup: {
                    inline_keyboard: levelsKeyboard()
                }
            }
        );

    } catch (error) {

        console.error('Start error:', error.message);
    }
});

// ==========================================
// 10. أمر الإحصائيات للمشرف
// ==========================================

bot.onText(/^\/stats(?:@\w+)?$/, async (msg) => {

    try {

        const adminId = process.env.ADMIN_ID;

        if (!adminId) {

            await bot.sendMessage(
                msg.chat.id,
                'خطأ: ADMIN_ID غير موجود في إعدادات البيئة.'
            );

            return;
        }

        if (
            String(msg.from.id) !==
            String(adminId).trim()
        ) {

            await bot.sendMessage(
                msg.chat.id,
                'غير مسموح لك بعرض الإحصاءات.'
            );

            return;
        }

        const users = Object.entries(
            stats.users || {}
        );

        const now = Date.now();

        const sevenDaysAgo =
            now - 7 * 24 * 60 * 60 * 1000;

        const twentyFourHoursAgo =
            now - 24 * 60 * 60 * 1000;

        const newUsers = users.filter(
            ([id, user]) => {
                return (
                    new Date(user.firstSeen).getTime() >=
                    sevenDaysAgo
                );
            }
        ).length;

        const activeUsers = users
            .filter(([id, user]) => {
                return (
                    new Date(user.lastSeen).getTime() >=
                    twentyFourHoursAgo
                );
            })
            .sort((a, b) => {
                return (
                    new Date(b[1].lastSeen) -
                    new Date(a[1].lastSeen)
                );
            });

        const topSubjects = Object.entries(
            stats.subjects || {}
        )
            .sort((a, b) => b[1] - a[1])
            .slice(0, 10);

        let message =
            '📊 إحصاءات Student DZ\n\n' +

            `👥 إجمالي المستخدمين: ${users.length}\n` +

            `🆕 المستخدمون الجدد خلال 7 أيام: ${newUsers}\n` +

            `🟢 النشطون خلال 24 ساعة: ${activeUsers.length}\n` +

            `📤 الملفات المرسلة بنجاح: ${stats.filesSent}\n\n`;

        message += '🏆 أكثر 10 مقاييس طلبًا:\n\n';

        if (topSubjects.length === 0) {

            message += 'لا توجد طلبات مسجلة بعد.\n';

        } else {

            topSubjects.forEach(
                ([subject, count], index) => {

                    message +=
                        `${index + 1}. ${subject}: ${count} طلب\n`;
                }
            );
        }

        message += '\n━━━━━━━━━━━━━━━━━━\n';

        message += '👤 المستخدمون النشطون (آخر 20):\n\n';

        if (activeUsers.length === 0) {

            message +=
                'لا يوجد مستخدمون نشطون خلال آخر 24 ساعة.';

        } else {

            activeUsers.slice(0, 20).forEach(
                ([id, user], index) => {

                    const fullName =
                        [
                            user.first_name,
                            user.last_name
                        ]
                            .filter(Boolean)
                            .join(' ') ||
                        'اسم غير متوفر';

                    const username = user.username
                        ? `@${user.username}`
                        : 'لا يوجد username';

                    message +=
                        `${index + 1}. ${fullName}\n` +
                        `   ${username}\n` +
                        `   ID: ${id}\n` +
                        `   آخر نشاط: ${formatDate(user.lastSeen)}\n\n`;
                }
            );
        }

        await bot.sendMessage(
            msg.chat.id,
            message
        );

    } catch (error) {

        console.error('Stats error:', error.message);

        await bot.sendMessage(
            msg.chat.id,
            'حدث خطأ أثناء عرض الإحصائيات.'
        );
    }
});

// ==========================================
// 11. التعامل مع الأزرار
// ==========================================

bot.on('callback_query', async (query) => {

    try {

        // الإجابة عن ضغط الزر فورًا
        // حتى لا يبقى المستخدم منتظرًا

        await bot.answerCallbackQuery(query.id).catch(() => {});

        if (!query.message) return;

        const chatId = query.message.chat.id;

        const choice = query.data;

        // --------------------------------------
        // التحقق من الاشتراك
        // --------------------------------------

        if (choice === 'check_subscription') {

            const subscription = await isSubscribed(
                query.from.id
            );

            if (subscription === null) {

                await bot.sendMessage(
                    chatId,
                    'تعذّر التحقق من الاشتراك. حاول مجددًا بعد قليل.'
                );

                return;
            }

            if (!subscription) {

                await sendSubscriptionPrompt(chatId);

                return;
            }

            recordActivity(query.from);

            await bot.sendMessage(
                chatId,

                '✅ تم التحقق من اشتراكك بنجاح!\n\nاختر مستواك:',

                {
                    reply_markup: {
                        inline_keyboard: levelsKeyboard()
                    }
                }
            );

            return;
        }

        const subscription = await isSubscribed(
            query.from.id
        );

        if (subscription === null) {

            await bot.sendMessage(
                chatId,
                'تعذّر التحقق من الاشتراك حاليًا. حاول مجددًا بعد قليل.'
            );

            return;
        }

        if (!subscription) {

            await sendSubscriptionPrompt(chatId);

            return;
        }

        recordActivity(query.from);

        // --------------------------------------
        // العودة إلى قائمة المستويات
        // --------------------------------------

        if (choice === 'back_levels') {

            await bot.sendMessage(
                chatId,
                'اختر مستواك:',
                {
                    reply_markup: {
                        inline_keyboard: levelsKeyboard()
                    }
                }
            );

            return;
        }

        // --------------------------------------
        // اختيار المستوى
        // --------------------------------------

        if (choice.startsWith('level_')) {

            const level = choice.slice(
                'level_'.length
            );

            if (semesters[level]) {
                await showSemesters(chatId, level);
            }

            return;
        }

        // --------------------------------------
        // اختيار السداسي
        // --------------------------------------

        if (choice.startsWith('semester_')) {

            const [, level, semester] =
                choice.split('_');

            if (!semesters[level]?.[semester]) {

                await bot.sendMessage(
                    chatId,
                    'سيتم إضافة مقاييس هذا الفصل لاحقًا.'
                );

                return;
            }

            await showSubjects(
                chatId,
                level,
                semester,
                0
            );

            return;
        }

        // --------------------------------------
        // التنقل بين صفحات المقاييس
        // --------------------------------------

        if (choice.startsWith('page_')) {

            const [, level, semester, pageText] =
                choice.split('_');

            await showSubjects(
                chatId,
                level,
                semester,
                Number(pageText)
            );

            return;
        }

        // --------------------------------------
        // اختيار المقياس
        // --------------------------------------

        if (choice.startsWith('subject_')) {

            const [, level, semester, indexText] =
                choice.split('_');

            const index = Number(indexText);

            const subject =
                semesters[level]?.[semester]?.subjects[index];

            if (!subject) {

                await bot.sendMessage(
                    chatId,
                    'لم أتمكن من العثور على هذا المقياس.'
                );

                return;
            }

            const keyboard = [];

            for (
                let i = 0;
                i < contentTypes.length;
                i += 2
            ) {

                keyboard.push(
                    contentTypes.slice(i, i + 2).map(
                        item => ({
                            text: item.label,

                            callback_data:
                                `content_${level}_${semester}_${index}_${item.id}`
                        })
                    )
                );
            }

            keyboard.push([
                {
                    text: '⬅️ العودة إلى المقاييس',

                    callback_data:
                        `page_${level}_${semester}_0`
                }
            ]);

            await bot.sendMessage(
                chatId,

                `📘 ${subject}\n\nاختر تصنيف الملفات:`,

                {
                    reply_markup: {
                        inline_keyboard: keyboard
                    }
                }
            );

            return;
        }

        // --------------------------------------
        // اختيار تصنيف الملفات وإرسالها
        // --------------------------------------

        if (choice.startsWith('content_')) {

            const [
                ,
                level,
                semester,
                indexText,
                contentTypeId
            ] = choice.split('_');

            const index = Number(indexText);

            const subject =
                semesters[level]?.[semester]?.subjects[index];

            const selectedType = contentTypes.find(
                item => item.id === contentTypeId
            );

            if (!subject || !selectedType) {

                await bot.sendMessage(
                    chatId,
                    'حدث خطأ في اختيار التصنيف.'
                );

                return;
            }

            // تسجيل طلب المقياس
            stats.subjects[subject] =
                (stats.subjects[subject] || 0) + 1;

            saveStats();

            const backButton = {
                reply_markup: {
                    inline_keyboard: [
                        [
                            {
                                text: '⬅️ العودة إلى التصنيفات',

                                callback_data:
                                    `subject_${level}_${semester}_${index}`
                            }
                        ]
                    ]
                }
            };

            // البحث عن الملفات
            const files = getAvailableFiles(
                level,
                semester,
                index,
                contentTypeId
            );

            if (files.length === 0) {

                await bot.sendMessage(
                    chatId,

                    `📘 ${subject}\n\nلا توجد ملفات ضمن تصنيف «${selectedType.label}» حاليًا.`,

                    backButton
                );

                return;
            }

            // إعلام المستخدم بعدد الملفات
            await bot.sendMessage(
                chatId,
                `📤 جارٍ إرسال ${files.length} ملف(ات)...`
            );

            let sentCount = 0;

            let failedCount = 0;

            // إرسال الملفات بالتتابع
            for (let i = 0; i < files.length; i++) {

                const file = files[i];

                if (!fs.existsSync(file.filePath)) {

                    console.error(
                        'File not found:',
                        file.filePath
                    );

                    failedCount++;

                    continue;
                }

                if (file.size > MAX_FILE_SIZE) {

                    await bot.sendMessage(
                        chatId,
                        `⚠️ الملف كبير جدًا ولا يمكن إرساله: ${file.name}`
                    );

                    failedCount++;

                    continue;
                }

                const caption =
                    `📘 ${subject}\n` +
                    `📄 ${file.name}\n` +
                    `النوع: ${selectedType.label}\n` +
                    `السنة: ${file.year}`;

                const sent = await sendFileSafely(
                    chatId,
                    file,
                    caption
                );

                if (sent) {

                    sentCount++;

                    stats.filesSent += 1;

                    saveStats();

                } else {

                    failedCount++;

                    await bot.sendMessage(
                        chatId,
                        `⚠️ تعذر إرسال الملف: ${file.name}`
                    );
                }

                // تأخير قصير فقط بين الملفات
                if (i < files.length - 1) {
                    await sleep(SEND_DELAY_MS);
                }
            }

            // ----------------------------------
            // انتهاء إرسال الملفات
            // ----------------------------------

            let finalMessage =
                `✅ انتهى إرسال الملفات.\n\n` +
                `📤 تم إرسال: ${sentCount}\n`;

            if (failedCount > 0) {
                finalMessage +=
                    `⚠️ تعذر إرسال: ${failedCount}\n`;
            }

            finalMessage +=
                '\nيمكنك اختيار تصنيف آخر.';

            await bot.sendMessage(
                chatId,
                finalMessage,
                backButton
            );

            return;
        }

    } catch (error) {

        console.error(
            'Callback error:',
            error.message
        );
    }
});

// ==========================================
// 12. معالجة الأخطاء
// ==========================================

process.on('unhandledRejection', (error) => {

    console.error(
        'Unhandled rejection:',
        error?.message || error
    );
});

bot.on('polling_error', (error) => {

    console.error(
        'Polling error:',
        error.message
    );
});

// ==========================================
// 13. تشغيل البوت
// ==========================================

console.log('Student DZ is running!');
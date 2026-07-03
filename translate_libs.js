const fs = require('fs');
const path = require('path');

const baseDir = 'D:\\9706_draintank_EN';
const tablePath = baseDir + '\\MainFrame\\HIFUMainFrameWithOperatorNavigation\\ENGLISH_TRANSLATION_TABLE.md';

// Parse translation table
const tableContent = fs.readFileSync(tablePath, 'utf8');
const lines = tableContent.split('\n');

const rawMap = new Map();
let inTable = false;

for (const line of lines) {
    if (/^\| (?:中|Chinese) \| (?:英|Recommended English)/.test(line)) {
        inTable = true; continue;
    }
    if (/^\|[\s\-|]+\|$/.test(line) && /-/.test(line)) continue;
    if (/^## /.test(line)) { inTable = false; continue; }
    if (inTable) {
        const m = line.match(/^\| (.+?) \| (.+?) \|/);
        if (m) {
            let c = m[1].trim(), e = m[2].trim();
            if (c && e !== 'Notes' && e !== 'Recommended English' && !/^-+$/.test(c)) {
                rawMap.set(c, e);
            }
        }
    }
}

// Multi-char entries: sort by length DESC
const multiSorted = [...rawMap.entries()]
    .filter(([c]) => c.length > 1)
    .sort((a, b) => b[0].length - a[0].length || a[0].localeCompare(b[0]));

// Single-char with boundary check
const singleEntries = [...rawMap.entries()].filter(([c]) => c.length === 1);

const libFiles = [
    'HIFUPlan\\PlanScene3D\\qvtkwrap.cpp',
    'HIFUPlan\\NewScene3D\\qvtkwrap.cpp',
    'HIFUPlan\\HifuImgaeAlgo\\hifuimagealgo.cpp',
    'HIFUConnection\\HIFUConnection\\maindialog.cpp',
    'HIFUNavigation\\HIFUNavigation\\maindialog.cpp',
    'HIFUNetwork\\Client\\client.cpp'
];

let modified = 0;
for (const relPath of libFiles) {
    const fullPath = baseDir + '\\' + relPath;
    if (!fs.existsSync(fullPath)) {
        console.log('NOT FOUND: ' + relPath);
        continue;
    }
    let content = fs.readFileSync(fullPath, 'utf8');
    const hadBom = content.charCodeAt(0) === 0xFEFF;
    if (hadBom) content = content.slice(1);
    const original = content;
    let changed = false;

    // Multi-char entries first
    for (const [chinese, english] of multiSorted) {
        if (content.includes(chinese)) {
            const escaped = chinese.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            content = content.replace(new RegExp(escaped, 'g'), english);
            changed = true;
        }
    }

    // Single-char with CJK boundary
    for (const [ch, eng] of singleEntries) {
        const escaped = ch.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
        const re = new RegExp('(?<![\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff])' + escaped + '(?![\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff])', 'g');
        if (re.test(content)) {
            content = content.replace(re, eng);
            changed = true;
        }
    }

    if (changed) {
        const buf = Buffer.from(content, 'utf8');
        fs.writeFileSync(fullPath, hadBom ? Buffer.concat([Buffer.from([0xEF, 0xBB, 0xBF]), buf]) : buf);
        modified++;
        console.log('  Modified: ' + relPath);
    } else {
        console.log('  No changes: ' + relPath);
    }
}

console.log('\nModified ' + modified + ' files.');

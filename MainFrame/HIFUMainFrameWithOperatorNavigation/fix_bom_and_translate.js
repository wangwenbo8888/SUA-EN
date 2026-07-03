const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const sourceDir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';

// Restore from git first
console.log('Restoring from git...');
execSync('git checkout -- .', { cwd: sourceDir, stdio: 'pipe' });

// Re-read the translation table
const tablePath = 'D:\\9706_draintank_EN\\ENGLISH_TRANSLATION_TABLE.md';
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

// Filter single-char and short substring entries
const filteredMap = new Map();
for (const [c, e] of rawMap) {
    if (c.length > 1) filteredMap.set(c, e);
}
const keys = [...filteredMap.keys()];
for (const k of keys) {
    if (k.length <= 2 && keys.some(lk => lk !== k && lk.includes(k))) {
        filteredMap.delete(k);
    }
}

const sorted = [...filteredMap.entries()].sort((a, b) => b[0].length - a[0].length);

console.log('Entries: ' + filteredMap.size);

// Walk source files
function walkDir(dir) {
    const files = [];
    for (const e of fs.readdirSync(dir, { withFileTypes: true })) {
        const p = path.join(dir, e.name);
        if (e.isDirectory()) {
            if (!['debug', 'release', 'build'].includes(e.name)) files.push(...walkDir(p));
        } else if (/\.(cpp|h|qml|ui)$/.test(e.name) && !/^(moc_|qrc_|ui_)/.test(e.name)) {
            files.push(p);
        }
    }
    return files;
}

const allFiles = walkDir(sourceDir);
let modified = 0;

for (const filePath of allFiles) {
    let content = fs.readFileSync(filePath, 'utf8');

    // Strip U+FEFF (BOM) from content if present
    const hadBom = content.charCodeAt(0) === 0xFEFF;
    if (hadBom) content = content.slice(1);

    const original = content;
    let changed = false;

    for (const [chinese, english] of sorted) {
        if (content.includes(chinese)) {
            const escaped = chinese.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            content = content.replace(new RegExp(escaped, 'g'), english);
            changed = true;
        }
    }

    if (changed) {
        const buf = Buffer.from(content, 'utf8');
        // Write with BOM if original had BOM
        fs.writeFileSync(filePath, hadBom ? Buffer.concat([Buffer.from([0xEF, 0xBB, 0xBF]), buf]) : buf);
        modified++;
        console.log('  ' + path.basename(filePath));
    }
}

console.log('\nDone! Modified ' + modified + ' files.');

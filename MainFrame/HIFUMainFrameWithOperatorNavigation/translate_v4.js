const fs = require('fs');
const path = require('path');
const { execSync } = require('child_process');

const sourceDir = 'D:\\9706_draintank_EN\\MainFrame\\HIFUMainFrameWithOperatorNavigation';
const tablePath = 'D:\\9706_draintank_EN\\ENGLISH_TRANSLATION_TABLE.md';

// Restore from git (skip if files are locked by another process)
console.log('Restoring from git...');
try {
    execSync('git checkout -- .', { cwd: sourceDir, stdio: 'pipe' });
} catch (e) {
    console.log('Warning: git restore failed (files may be locked). Applying on top of current state.');
}

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

// Separate multi-char and single-char entries
const multiMap = new Map();
const singleMap = new Map();
for (const [c, e] of rawMap) {
    if (c.length > 1) multiMap.set(c, e);
    else singleMap.set(c, e); // single-char tracked separately for bounded matching
}

// Multi-char: sorted by length DESCENDING (longest first prevents substring corruption)
const multiSorted = [...multiMap.entries()].sort((a, b) => b[0].length - a[0].length || a[0].localeCompare(b[0]));

// Single-char: will match only when not adjacent to other CJK chars
const singleSorted = [...singleMap.entries()];

console.log('Multi-char entries: ' + multiSorted.length);
console.log('Single-char entries: ' + singleSorted.length);
if (multiSorted.length > 0) {
  console.log('Longest multi: [' + multiSorted[0][0] + '] (' + multiSorted[0][0].length + ' chars) -> ' + multiSorted[0][1]);
  console.log('Shortest multi: [' + multiSorted[multiSorted.length-1][0] + '] (' + multiSorted[multiSorted.length-1][0].length + ' chars) -> ' + multiSorted[multiSorted.length-1][1]);
}

// Walk source files
function walkDir(dir) {
    const files = [];
    for (const e of fs.readdirSync(dir, { withFileTypes: true })) {
        const p = path.join(dir, e.name);
        if (e.isDirectory()) {
            if (!['debug', 'release', 'build', '.git', '.vs', '.agents'].includes(e.name)) files.push(...walkDir(p));
        } else if (/\.(cpp|h|qml|ui)$/.test(e.name) && !/^(moc_|qrc_|ui_)/.test(e.name) && !/\.(js|ps1)$/.test(e.name)) {
            files.push(p);
        }
    }
    return files;
}

const allFiles = walkDir(sourceDir);
let modified = 0;

for (const filePath of allFiles) {
    let content = fs.readFileSync(filePath, 'utf8');

    // Strip BOM
    const hadBom = content.charCodeAt(0) === 0xFEFF;
    if (hadBom) content = content.slice(1);

    const original = content;
    let changed = false;

    // Phase 1: apply multi-char entries (length DESC, safe)
    for (const [chinese, english] of multiSorted) {
        if (content.includes(chinese)) {
            const escaped = chinese.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
            content = content.replace(new RegExp(escaped, 'g'), english);
            changed = true;
        }
    }

    // Phase 2: apply single-char entries ONLY when not adjacent to other CJK
    for (const [ch, eng] of singleSorted) {
        const escaped = ch.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
        const re = new RegExp('(?<![\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff])' + escaped + '(?![\u4e00-\u9fff\u3400-\u4dbf\uf900-\ufaff])', 'g');
        if (re.test(content)) {
            content = content.replace(re, eng);
            changed = true;
        }
    }

    if (changed) {
        const buf = Buffer.from(content, 'utf8');
        fs.writeFileSync(filePath, hadBom ? Buffer.concat([Buffer.from([0xEF, 0xBB, 0xBF]), buf]) : buf);
        modified++;
        console.log('  ' + path.basename(filePath));
    }
}

console.log('\nModified ' + modified + ' files.');

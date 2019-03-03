/*
Word: kirsa
Results: irask,kirsa,raisk,rakis,rikas,sakri
71332.42 microseconds
0.071332 seconds
*/
const fs = require('fs');
const rl = require('readline');
const Iconv = require('iconv').Iconv;

const methods = {
    start: process.hrtime(),
    elapsedTime: () => {
        let end = process.hrtime();
        let elapsed = (end[0] * 1000000 + end[1] / 1000) - (methods.start[0] * 1000000 + methods.start[1] / 1000);
        return elapsed.toFixed(2);
    }
}
if (process.argv.length < 4)
{
    console.log("Usaeg: " + process.argv[0] + " " + process.argv[1] + " /path/to/dictionary word");
    process.exit();
}
const dictionaryFileName = process.argv[2];
let word = process.argv[3];

let stat = {};
for (let i = 0; i < word.length; i++)
{
    //let char = word[i].toLowerCase();
    let char = word[i];
    stat[char] = !(char in stat) ? 1 : ++stat[char];
}

const lineReader = rl.createInterface({
    input: fs.createReadStream(dictionaryFileName, { 
        'flags': 'r',
        'encoding': 'binary'
    })
});

let conv = new Iconv('windows-1257', 'utf8');
let results = [];
lineReader.on('line', (line) => {
    if (line.length !== word.length || line === word)
    {
        return;
    }
    line = new Buffer(line, 'binary');
    line = conv.convert(line).toString("utf8");
    let s = Object.assign({}, stat);
    
    for (let i = 0; i < line.length; i++)
    {
        //let char = line[i].toLowerCase();
        let char = line[i];
        if (!s[char])
        {
            return;
        }
        s[char]--;
    }
    results.push(line);
});

lineReader.on('close', () => {
    let elapsed = methods.elapsedTime();
    console.log(Math.floor(elapsed) + (results.length > 0 ? ',':'') + results);
});

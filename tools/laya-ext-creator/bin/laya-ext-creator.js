#!/usr/bin/env node

const createVsProject = require('../index');
const readline = require('readline');

const rl = readline.createInterface({
    input: process.stdin,
    output: process.stdout
});

rl.question('请输入项目名称: ', (projectName) => {
    rl.question('请输入输出路径 (直接回车使用当前目录): ', (outputPath) => {
        outputPath = outputPath || '.';
        createVsProject(projectName, outputPath);
        rl.close();
        process.exit(0);
    });
});
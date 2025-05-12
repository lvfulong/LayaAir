const express = require('express');
const multer = require('multer');
const path = require('path');
const fs = require('fs');

const app = express();
const port = 3000;

// 配置文件存储
const storage = multer.diskStorage({
    destination: function (req, file, cb) {
        const uploadDir = path.join(__dirname, 'uploads');
        if (!fs.existsSync(uploadDir)) {
            fs.mkdirSync(uploadDir);
        }
        cb(null, uploadDir);
    },
    filename: function (req, file, cb) {
        // 保持原始文件名
        cb(null, file.originalname);
    }
});

// 创建 multer 实例
const upload = multer({ 
    storage: storage,
    limits: {
        fileSize: 50 * 1024 * 1024  // 限制50MB
    }
});

// 模拟上传延迟
const simulateDelay = (req, res, next) => {
    // 随机延迟 0-2 秒
    setTimeout(next, Math.random() * 2000);
};

// 处理文件上传
app.post('/upload', simulateDelay, upload.single('file'), (req, res) => {
    try {
        console.log(`upload file: ${req.file ? req.file.originalname : 'none'}`);
        // 获取上传的文件信息
        const file = req.file;
        if (!file) {
            return res.status(400).json({
                success: false,
                message: 'No file uploaded'
            });
        }

        // 获取其他表单字段
        const formData = req.body;

        // 返回成功响应
        res.json({
            success: true,
            message: 'File uploaded successfully',
            file: {
                filename: file.originalname,
                size: file.size,
                mimetype: file.mimetype
            },
            formData: formData
        });
    } catch (error) {
        console.log(`upload file error: ${error.stack}`);
        res.status(500).json({
            success: false,
            message: error.message
        });
    }
});

// 错误处理中间件
app.use((err, req, res, next) => {
    console.error(err.stack);
    res.status(500).json({
        success: false,
        message: err.message
    });
});

// 启动服务器
const server = app.listen(port, () => {
    console.log(`Test server running at http://localhost:${port}`);
});

// 优雅关闭
process.on('SIGTERM', () => {
    console.log('SIGTERM signal received: closing HTTP server');
    server.close(() => {
        console.log('HTTP server closed');
        // 清理上传的文件
        const uploadDir = path.join(__dirname, 'uploads');
        if (fs.existsSync(uploadDir)) {
            fs.rmSync(uploadDir, { recursive: true, force: true });
        }
    });
});

// 导出服务器实例，方便测试
module.exports = server;

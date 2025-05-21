#include "RTDynamicVIBuffer.h"

namespace laya
{

// BufferDataView实现
BufferDataView::BufferDataView(WebDynamicVIBuffer* owner, Float32Array* source, int start, int length, int stride) {
    this->owner = owner;
    this->_start = start;
    this->_length = length;
    this->_stride = stride;
    
    if (start + length > source->length()) {
        this->_data = new Float32Array(length);
        this->_isUsingTemp = true;
    } else {
        this->_data = new Float32Array(source->buffer(), start * source->BYTES_PER_ELEMENT, length);
    }
}

BufferDataView::~BufferDataView() {
    delete _data;
}

void BufferDataView::modify(BufferModifyType type) {
    if (type == BufferModifyType::Vertex) {
        owner->_vertexModify = true;
    } else {
        owner->_indexModify = true;
    }
    _isModified = true;
    owner->needUpload = true;
}

void BufferDataView::updateView(Float32Array* newData) {
    if (_isUsingTemp) {
        newData->set(_data, _start);
        _isUsingTemp = false;
    }
    delete _data;
    _data = new Float32Array(newData->buffer(), _start * newData->BYTES_PER_ELEMENT, _length);
}

DataRange BufferDataView::getDataRange() const {
    return {_start, _length};
}

// WebDynamicVIBuffer实现
WebDynamicVIBuffer::WebDynamicVIBuffer(int vertexBlockSize, int indexBlockSize) {
    _vertexBlockSize = vertexBlockSize;
    _indexBlockSize = indexBlockSize;
    
    int vertexDefaultSize = vertexBlockSize * DEFAULT_BLOCK_SIZE;
    _vertexData = new Float32Array(vertexDefaultSize);
    int indexDefaultSize = indexBlockSize * DEFAULT_BLOCK_SIZE;
    _indexData = new Uint16Array(indexDefaultSize);

    _vertexBuffer = LayaGL::renderDeviceFactory->createVertexBuffer(BufferUsage::Dynamic);
    _vertexBuffer->setDataLength(vertexDefaultSize * 4);
    _indexBuffer = LayaGL::renderDeviceFactory->createIndexBuffer(BufferUsage::Dynamic);
    _indexBuffer->setIndexType(IndexFormat::UInt16);
    _indexBuffer->setIndexDataLength(indexDefaultSize * 2);

    _bufferState = LayaGL::renderDeviceFactory->createBufferState();
}

WebDynamicVIBuffer::~WebDynamicVIBuffer() {
    destroy();
}

void WebDynamicVIBuffer::setVertexDeclaration(VertexDeclaration* vertexDeclaration) {
    if (_vertexDeclaration != vertexDeclaration) {
        _vertexDeclaration = vertexDeclaration;
        _vertexBuffer->setVertexDeclaration(vertexDeclaration);
        _bufferState->applyState({_vertexBuffer}, _indexBuffer);
    }
}

IBufferBlock WebDynamicVIBuffer::checkVertexBuffer(int length) {
    int requiredBlocks = (length + _vertexBlockSize - 1) / _vertexBlockSize;
    int needNewBlocks = requiredBlocks - _vertexFreeBlocks.size();

    if (needNewBlocks > 0) {
        int totalBlocks = _vertexViews.size() + needNewBlocks;
        int vertexCount = totalBlocks * _vertexBlockSize / _vertexDeclaration->getVertexStride() * 4;
        
        if (vertexCount > MAX_VERTEX) {
            return {nullptr, 0};
        }

        _vertexState = BufferState::NEED_RESIZE;
        _vertexData = new Float32Array(vertexCount);
    }

    int startBlock = _vertexFreeBlocks.empty() ? _vertexViews.size() : _vertexFreeBlocks.back();
    _vertexFreeBlocks.pop_back();

    return {new BufferDataView(this, _vertexData, startBlock * _vertexBlockSize, length), startBlock};
}

IBufferBlock WebDynamicVIBuffer::checkIndexBuffer(int length) {
    int requiredBlocks = (length + _indexBlockSize - 1) / _indexBlockSize;
    int needNewBlocks = requiredBlocks - _indexFreeBlocks.size();

    if (needNewBlocks > 0) {
        int totalBlocks = _indexViews.size() + needNewBlocks;
        int indexCount = totalBlocks * _indexBlockSize;
        
        if (indexCount > MAX_VERTEX) {
            return {nullptr, 0};
        }

        _indexState = BufferState::NEED_RESIZE;
        _indexData = new Uint16Array(indexCount);
    }

    int startBlock = _indexFreeBlocks.empty() ? _indexViews.size() : _indexFreeBlocks.back();
    _indexFreeBlocks.pop_back();

    return {new BufferDataView(this, _indexData, startBlock * _indexBlockSize, length), startBlock};
}

void WebDynamicVIBuffer::upload() {
    if (_vertexState == BufferState::NEED_RESIZE) {
        _vertexBuffer->setDataLength(_vertexData->length() * 4);
        _vertexState = BufferState::RESIZED;
    }
    
    if (_indexState == BufferState::NEED_RESIZE) {
        _indexBuffer->setIndexDataLength(_indexData->length() * 2);
        _indexState = BufferState::RESIZED;
    }

    if (_vertexModify) {
        _vertexBuffer->setData(_vertexData);
        _vertexModify = false;
    }

    if (_indexModify) {
        _indexBuffer->setIndexData(_indexData);
        _indexModify = false;
    }

    needUpload = false;
}

void WebDynamicVIBuffer::destroy() {
    if (_vertexBuffer) {
        _vertexBuffer->destroy();
        _vertexBuffer = nullptr;
    }
    if (_indexBuffer) {
        _indexBuffer->destroy();
        _indexBuffer = nullptr;
    }
    if (_bufferState) {
        _bufferState->destroy();
        _bufferState = nullptr;
    }
    if (_vertexData) {
        delete _vertexData;
        _vertexData = nullptr;
    }
    if (_indexData) {
        delete _indexData;
        _indexData = nullptr;
    }
    _vertexState = BufferState::NO_CHANGE;
    _indexState = BufferState::NO_CHANGE;
    _vertexFreeBlocks.clear();
    _indexFreeBlocks.clear();
    _vertexViews.clear();
    _indexViews.clear();
    _vertexModify = false;
    _indexModify = false;
    needUpload = false;
} 
} // namespace laya s
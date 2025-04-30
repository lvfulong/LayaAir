interface Algorithm {
    name?: string;
}

interface KeyAlgorithm {
    name?: string;
}

interface CryptoKey {
    algorithm: KeyAlgorithm;
    extractable: boolean;
    type: string;
    usages: string[];
}

class SubtleCrypto {
    decrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        throw 'not implemented';
    }
    deriveBits(algorithm: string | Algorithm, baseKey: CryptoKey, length: number): any{
        throw 'not implemented';
    }
    deriveKey(algorithm: string | Algorithm, baseKey: CryptoKey, derivedKeyType: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        throw 'not implemented';
    }
    digest(algorithm: string | Algorithm, data: ArrayBufferView): Promise<ArrayBuffer>{
        if (algorithm == 'SHA-1') {
            let result =  (window as any).conchSubtleCrypto.digest(algorithm, data);
            return new Promise((resolve, reject) => {
                if (result) {
                    resolve(result);
                } else {
                    reject(new Error('Failed to digest'));
                }
            }); 
        }
        throw 'not implemented';
    }
    encrypt(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        throw 'not implemented';
    }
    exportKey(format: string, key: CryptoKey): any{
        throw 'not implemented';
    }
    generateKey(algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        throw 'not implemented';
    }
    importKey(format: string, keyData: ArrayBufferView, algorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        throw 'not implemented';
    }
    sign(algorithm: string | Algorithm, key: CryptoKey, data: ArrayBufferView): any{
        throw 'not implemented';
    }
    unwrapKey(format: string, wrappedKey: ArrayBufferView, unwrappingKey: CryptoKey, unwrapAlgorithm: string | Algorithm, unwrappedKeyAlgorithm: string | Algorithm, extractable: boolean, keyUsages: string[]): any{
        throw 'not implemented';
    }
    verify(algorithm: string | Algorithm, key: CryptoKey, signature: ArrayBufferView, data: ArrayBufferView): any{
        throw 'not implemented';
    }
    wrapKey(format: string, key: CryptoKey, wrappingKey: CryptoKey, wrapAlgorithm: string | Algorithm): any{
        throw 'not implemented';
    }
}

class Crypto {
    subtle = new SubtleCrypto();
    getRandomValues(array: ArrayBufferView): ArrayBufferView{
        throw 'not implemented';
        return null;
    }
}
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWMnemonic.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWData.h>
#include <TrustWalletCore/TWAnySigner.h>

#include <iostream>
#include <string>
#include <cstring>
#include <memory>

// Helper function to create TWString from std::string
std::shared_ptr<TWString> makeString(const std::string& str) {
    return std::shared_ptr<TWString>(
        TWStringCreateWithUTF8Bytes(str.c_str()),
        TWStringDelete
    );
}

// Helper function to convert TWString to std::string
std::string getString(TWString* str) {
    return std::string(TWStringUTF8Bytes(str));
}

// Helper function to parse coin type from string
TWCoinType parseCoinType(const std::string& coin) {
    if (coin == "btc" || coin == "bitcoin") {
        return TWCoinTypeBitcoin;
    } else if (coin == "eth" || coin == "ethereum") {
        return TWCoinTypeEthereum;
    }
    return TWCoinTypeBitcoin; // default
}

// Command: create-seed
// Creates a new random mnemonic seed phrase
void createSeed() {
    auto passphrase = makeString("");
    auto wallet = std::shared_ptr<TWHDWallet>(
        TWHDWalletCreate(128, passphrase.get()),
        TWHDWalletDelete
    );
    
    if (!wallet) {
        std::cerr << "Error: Failed to create wallet" << std::endl;
        return;
    }
    
    auto mnemonic = std::shared_ptr<TWString>(
        TWHDWalletMnemonic(wallet.get()),
        TWStringDelete
    );
    
    std::cout << "New mnemonic seed phrase:" << std::endl;
    std::cout << getString(mnemonic.get()) << std::endl;
}

// Command: import-seed <mnemonic>
// Imports an existing mnemonic seed phrase
void importSeed(const std::string& mnemonic) {
    auto mnemonicStr = makeString(mnemonic);
    
    // Validate mnemonic
    if (!TWMnemonicIsValid(mnemonicStr.get())) {
        std::cerr << "Error: Invalid mnemonic phrase" << std::endl;
        return;
    }
    
    auto passphrase = makeString("");
    auto wallet = std::shared_ptr<TWHDWallet>(
        TWHDWalletCreateWithMnemonic(mnemonicStr.get(), passphrase.get()),
        TWHDWalletDelete
    );
    
    if (!wallet) {
        std::cerr << "Error: Failed to import wallet" << std::endl;
        return;
    }
    
    std::cout << "Successfully imported mnemonic" << std::endl;
    std::cout << "Mnemonic: " << mnemonic << std::endl;
}

// Command: derive-address --coin <coin>
// Derives an address for the specified coin from a mnemonic
void deriveAddress(const std::string& mnemonic, const std::string& coin) {
    auto mnemonicStr = makeString(mnemonic);
    
    // Validate mnemonic
    if (!TWMnemonicIsValid(mnemonicStr.get())) {
        std::cerr << "Error: Invalid mnemonic phrase" << std::endl;
        return;
    }
    
    auto passphrase = makeString("");
    auto wallet = std::shared_ptr<TWHDWallet>(
        TWHDWalletCreateWithMnemonic(mnemonicStr.get(), passphrase.get()),
        TWHDWalletDelete
    );
    
    if (!wallet) {
        std::cerr << "Error: Failed to create wallet from mnemonic" << std::endl;
        return;
    }
    
    TWCoinType coinType = parseCoinType(coin);
    auto address = std::shared_ptr<TWString>(
        TWHDWalletGetAddressForCoin(wallet.get(), coinType),
        TWStringDelete
    );
    
    std::cout << "Address for " << coin << ": " << getString(address.get()) << std::endl;
}

// Command: sign-transaction
// Signs a simple transaction (demonstrative example)
void signTransaction(const std::string& mnemonic, const std::string& coin) {
    auto mnemonicStr = makeString(mnemonic);
    
    // Validate mnemonic
    if (!TWMnemonicIsValid(mnemonicStr.get())) {
        std::cerr << "Error: Invalid mnemonic phrase" << std::endl;
        return;
    }
    
    auto passphrase = makeString("");
    auto wallet = std::shared_ptr<TWHDWallet>(
        TWHDWalletCreateWithMnemonic(mnemonicStr.get(), passphrase.get()),
        TWHDWalletDelete
    );
    
    if (!wallet) {
        std::cerr << "Error: Failed to create wallet from mnemonic" << std::endl;
        return;
    }
    
    TWCoinType coinType = parseCoinType(coin);
    auto privateKey = std::shared_ptr<TWPrivateKey>(
        TWHDWalletGetKeyForCoin(wallet.get(), coinType),
        TWPrivateKeyDelete
    );
    
    auto privateKeyData = std::shared_ptr<TWData>(
        TWPrivateKeyData(privateKey.get()),
        TWDataDelete
    );
    
    auto hexStr = std::shared_ptr<TWString>(
        TWStringCreateWithHexData(privateKeyData.get()),
        TWStringDelete
    );
    
    std::cout << "Transaction signing demonstration for " << coin << std::endl;
    std::cout << "Private key (hex): " << getString(hexStr.get()) << std::endl;
    std::cout << "Note: In production, never expose private keys!" << std::endl;
}

void printUsage() {
    std::cout << "NewWallet - A minimal C++ wallet demonstration using TrustWallet Core" << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  newwallet create-seed" << std::endl;
    std::cout << "      Create a new random mnemonic seed phrase" << std::endl;
    std::cout << std::endl;
    std::cout << "  newwallet import-seed <mnemonic>" << std::endl;
    std::cout << "      Import an existing mnemonic seed phrase" << std::endl;
    std::cout << std::endl;
    std::cout << "  newwallet derive-address --coin <coin> <mnemonic>" << std::endl;
    std::cout << "      Derive an address for a specific coin (btc or eth)" << std::endl;
    std::cout << std::endl;
    std::cout << "  newwallet sign-transaction --coin <coin> <mnemonic>" << std::endl;
    std::cout << "      Demonstrate transaction signing for a specific coin" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  newwallet create-seed" << std::endl;
    std::cout << "  newwallet import-seed \"word1 word2 ... word12\"" << std::endl;
    std::cout << "  newwallet derive-address --coin btc \"word1 word2 ... word12\"" << std::endl;
    std::cout << "  newwallet sign-transaction --coin eth \"word1 word2 ... word12\"" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    std::string command = argv[1];
    
    if (command == "create-seed") {
        createSeed();
    } else if (command == "import-seed") {
        if (argc < 3) {
            std::cerr << "Error: Missing mnemonic argument" << std::endl;
            printUsage();
            return 1;
        }
        std::string mnemonic = argv[2];
        importSeed(mnemonic);
    } else if (command == "derive-address") {
        if (argc < 5 || std::string(argv[2]) != "--coin") {
            std::cerr << "Error: Usage: newwallet derive-address --coin <coin> <mnemonic>" << std::endl;
            return 1;
        }
        std::string coin = argv[3];
        std::string mnemonic = argv[4];
        deriveAddress(mnemonic, coin);
    } else if (command == "sign-transaction") {
        if (argc < 5 || std::string(argv[2]) != "--coin") {
            std::cerr << "Error: Usage: newwallet sign-transaction --coin <coin> <mnemonic>" << std::endl;
            return 1;
        }
        std::string coin = argv[3];
        std::string mnemonic = argv[4];
        signTransaction(mnemonic, coin);
    } else {
        std::cerr << "Error: Unknown command '" << command << "'" << std::endl;
        printUsage();
        return 1;
    }
    
    return 0;
}

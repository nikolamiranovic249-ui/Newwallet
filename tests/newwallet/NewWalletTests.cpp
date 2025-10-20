// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"

#include <TrustWalletCore/TWHDWallet.h>
#include <TrustWalletCore/TWMnemonic.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWString.h>

#include <gtest/gtest.h>

using namespace TW;

TEST(NewWallet, CreateSeed) {
    auto passphrase = STRING("");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreate(128, passphrase.get()));
    
    ASSERT_NE(wallet, nullptr);
    
    auto mnemonic = WRAPS(TWHDWalletMnemonic(wallet.get()));
    ASSERT_TRUE(TWMnemonicIsValid(mnemonic.get()));
}

TEST(NewWallet, ImportSeed) {
    const char* mnemonicStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    auto mnemonic = STRING(mnemonicStr);
    
    ASSERT_TRUE(TWMnemonicIsValid(mnemonic.get()));
    
    auto passphrase = STRING("");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));
    
    ASSERT_NE(wallet, nullptr);
    
    auto recoveredMnemonic = WRAPS(TWHDWalletMnemonic(wallet.get()));
    assertStringsEqual(recoveredMnemonic, mnemonicStr);
}

TEST(NewWallet, DeriveAddressBitcoin) {
    const char* mnemonicStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    auto mnemonic = STRING(mnemonicStr);
    auto passphrase = STRING("");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));
    
    ASSERT_NE(wallet, nullptr);
    
    auto address = WRAPS(TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeBitcoin));
    
    // Verify we got an address
    ASSERT_GT(TWStringSize(address.get()), 0);
    
    // Bitcoin address should start with bc1 (bech32) or 1/3 (legacy)
    std::string addressStr = TWStringUTF8Bytes(address.get());
    ASSERT_FALSE(addressStr.empty());
}

TEST(NewWallet, DeriveAddressEthereum) {
    const char* mnemonicStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    auto mnemonic = STRING(mnemonicStr);
    auto passphrase = STRING("");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));
    
    ASSERT_NE(wallet, nullptr);
    
    auto address = WRAPS(TWHDWalletGetAddressForCoin(wallet.get(), TWCoinTypeEthereum));
    
    // Verify we got an address
    ASSERT_GT(TWStringSize(address.get()), 0);
    
    // Ethereum address should start with 0x
    std::string addressStr = TWStringUTF8Bytes(address.get());
    ASSERT_EQ(addressStr.substr(0, 2), "0x");
}

TEST(NewWallet, GetPrivateKeyForCoin) {
    const char* mnemonicStr = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    auto mnemonic = STRING(mnemonicStr);
    auto passphrase = STRING("");
    auto wallet = WRAP(TWHDWallet, TWHDWalletCreateWithMnemonic(mnemonic.get(), passphrase.get()));
    
    ASSERT_NE(wallet, nullptr);
    
    // Get private key for Bitcoin
    auto privateKey = WRAP(TWPrivateKey, TWHDWalletGetKeyForCoin(wallet.get(), TWCoinTypeBitcoin));
    ASSERT_NE(privateKey, nullptr);
    
    auto privateKeyData = WRAPD(TWPrivateKeyData(privateKey.get()));
    ASSERT_EQ(TWDataSize(privateKeyData.get()), 32); // Private key should be 32 bytes
}

TEST(NewWallet, InvalidMnemonic) {
    auto invalidMnemonic = STRING("invalid mnemonic phrase that should not work");
    
    ASSERT_FALSE(TWMnemonicIsValid(invalidMnemonic.get()));
}

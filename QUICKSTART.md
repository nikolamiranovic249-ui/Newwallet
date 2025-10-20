# Quick Start Guide - Trust Wallet Core

This guide will help you quickly install, build, and test the Trust Wallet Core on Linux.

## Prerequisites

- Ubuntu 24.04 (or similar Linux distribution)
- At least 4GB RAM
- At least 10GB free disk space
- Internet connection

## Installation Steps

### 1. Install System Dependencies

```bash
# Install system packages
tools/install-sys-dependencies-linux
```

This installs:
- ninja-build
- llvm-14, clang-14, clang-tidy-14
- libboost-all-dev
- gcc-multilib, g++-multilib
- And other required packages

### 2. Install Rust Dependencies

```bash
# Install Rust toolchain and tools
tools/install-rust-dependencies dev
```

This installs:
- Rust nightly toolchain
- Cross-compilation targets
- cbindgen and cargo-llvm-cov

### 3. Install Internal Dependencies

```bash
# Install C++ libraries (gtest, protobuf, etc.)
tools/install-dependencies
```

This builds and installs:
- Google Test framework
- Protocol Buffers v3.20.3
- libcheck
- nlohmann_json

### 4. Generate Files

```bash
# Generate protobuf files and coin registry
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
tools/generate-files native
```

### 5. Build Rust Components

```bash
cd rust
cargo build --release
cd ..
```

### 6. Configure and Build C++ Components

```bash
# Configure with CMake
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Debug -GNinja

# Build walletconsole
ninja -Cbuild walletconsole
```

## Running the Wallet Console

Once built, you can run the interactive wallet console:

```bash
./build/walletconsole/walletconsole
```

## Basic Usage Examples

### Example 1: Generate a Bitcoin Address

```
> coin bitcoin
Set active coin to: bitcoin
> newKey
Result: [your private key]
> addrPri #
Result: [your Bitcoin address]
```

### Example 2: Create Mnemonic and Derive Addresses

```
> newMnemonic 128
New mnemonic set.
> coin ethereum
Set active coin to: ethereum
> addrDefault
Result: [your Ethereum address]
> coin solana
Set active coin to: solana
> addrDefault
Result: [your Solana address]
```

### Example 3: List All Supported Coins

```
> coins
[Lists all 162 supported blockchains]
```

## Available Commands

- `help` - Show all available commands
- `coins` - List all supported coins
- `coin <name>` - Switch to a specific coin
- `newKey` - Generate a new private key
- `newMnemonic <bits>` - Create a new mnemonic (128-256 bits)
- `addrPri <key>` - Generate address from private key
- `addrDefault` - Derive default address from mnemonic
- `exit` or `quit` - Exit the console

## Supported Blockchains (162 total)

Including but not limited to:
- **Bitcoin & Forks:** Bitcoin, Bitcoin Cash, Litecoin, Dogecoin, Zcash
- **Ethereum & EVM Chains:** Ethereum, Polygon, BSC, Arbitrum, Optimism, Base
- **Layer 1 Blockchains:** Solana, Aptos, Sui, Cardano, Polkadot, Cosmos
- **And 140+ more!**

## Building Tests (Optional)

To run the full test suite:

```bash
ninja -Cbuild tests
build/tests/tests
```

To run Trezor crypto tests:

```bash
build/trezor-crypto/crypto/tests/TrezorCryptoTests
```

## Using the All-in-One Script

Alternatively, you can use the bootstrap script to install everything:

```bash
./bootstrap.sh
```

Then build and test with:

```bash
tools/build-and-test
```

## Troubleshooting

### Issue: Missing protoc

If you get "protoc: command not found", make sure you've run `tools/install-dependencies` which installs protobuf to `build/local/bin/protoc`.

### Issue: Clang not found

Make sure clang is installed:
```bash
sudo apt-get install clang
```

### Issue: Build fails with missing Rust library

Make sure you've built the Rust components first:
```bash
cd rust && cargo build --release && cd ..
```

## Security Warning

⚠️ **IMPORTANT:** Never share your private keys or mnemonic phrases. The wallet console marks all secret data with "(secret!)" in the help text. Always keep these values secure.

## Next Steps

- Read the full documentation at https://developer.trustwallet.com/wallet-core
- Explore the API for integrating into your applications
- Check out sample projects in the `samples/` directory
- Review security audit reports in the `audit/` directory

## Support

- GitHub Issues: https://github.com/trustwallet/wallet-core/issues
- Documentation: https://developer.trustwallet.com/wallet-core
- Contributing Guide: https://developer.trustwallet.com/wallet-core/contributing

---

**Happy Building! 🚀**

#include <ctime>    // gives functions to manipulate date time objects
#include <vector>   // data structure similar to an array do not have to allocate mem at design time (noob stuff)
#include <string>
#include <iostream>

using namespace std;

// Transaction data
struct TransactionData {
    double amount;
    string sendKey;
    string recvKey;
    time_t timeStamp;
};

// Block Class
class Block {
    private:
        int index;
        size_t blockHash;
        size_t previousHash;
        size_t generateHash();

    public:
        // Constructor 
        Block(int index, TransactionData data, size_t previousHash);

        // Get original hash
        size_t getHash();

        // Get previous hash
        size_t getPreviousHash();

        // Transaction data
        TransactionData data;

        // Validate hash
        bool isHashValid();
};

// Constructor with params
Block::Block(int idx, TransactionData d, size_t prevHash) {
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();
}

// Private functions 
size_t Block::generateHash() {
    hash<string> hash1;
    hash<size_t> hash2;
    hash<size_t> finalHash;
    string toHash = to_string(data.amount) + data.recvKey + data.sendKey + to_string(data.timeStamp);

    return finalHash(hash1(toHash) + hash2(previousHash));
};

// Public functions 
size_t Block::getHash() {
    return blockHash;
}

size_t Block::getPreviousHash() {
    return previousHash;
}

bool Block::isHashValid() {
    return generateHash() == blockHash;
}

// Blockchain class
class Blockchain {
    private:
        Block createGenesisBlock();

    public:
        // Public chain
        vector<Block> chain;

        // Constructor 
        Blockchain();

        // Public functions

        void addBlock(TransactionData data);
        bool isChainValid();

        // Example for demo only
        Block *getLatestBlock();    // returns pointer to fake manipulate the data in the block, to use as test to verify the integrity

};

// Blockchain Constructor
Blockchain::Blockchain() {
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
}

// Create genesis block 
Block Blockchain::createGenesisBlock() {
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.recvKey = "My balls";
    d.sendKey = "Also my balls";
    d.timeStamp = time(&current);

    hash<int> hash1;
    Block genesis(0, d, hash1(0));
    return genesis;
}

// Bad (dont do) only for demo
Block *Blockchain::getLatestBlock() {
    return &chain.back();
}

void Blockchain::addBlock(TransactionData d) {
    int index = (int)chain.size() - 1;
    Block newBlock(index, d, getLatestBlock()->getHash());
}

bool Blockchain::isChainValid() {
    vector<Block>::iterator it;
    int chainLength = (int)chain.size();

    for (it = chain.begin(); it != chain.end(); ++it) {
        Block currentBlock = *it;

        // Check if the current hash is valid 
        if (!currentBlock.isHashValid()) {
            return false;
        }

        // Check if the previous hash in current block is the current hash in the previous block (validating hashes in different blocks)
        if (chainLength > 1) {
            Block prevBlock = *(it -1);
            if (currentBlock.getPreviousHash() != prevBlock.getHash()) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    // Start Blockchain
    Blockchain testCoin;

    // Data for first added block
    TransactionData data1;
    time_t data1Time;
    data1.amount    =   8.9;
    data1.recvKey   =   "Josh";
    data1.sendKey    =   "Cael";
    data1.timeStamp =   time(&data1Time);

    testCoin.addBlock(data1);

    cout << "Is chain valid?" << endl
        << testCoin.isChainValid() << endl;

    // Second trans
    TransactionData data2;
    time_t data2Time;
    data2.amount    =   4.0;
    data2.recvKey   =   "Cael";
    data2.sendKey    =   "Aryaj";
    data2.timeStamp =   time(&data1Time);

    cout << "Now is the chain valid?" << endl
        << testCoin.isChainValid() << endl;

    // Someone is changing the mem vals
    Block *hacker = testCoin.getLatestBlock();
    hacker->data.amount = 100;
    hacker->data.recvKey = "Josh";

    cout << "Now is the chain valid2 ?" << endl;
    cout << testCoin.isChainValid() << endl;
}
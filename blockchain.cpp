#include <ctime>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

using namespace std;

//transition data
struct TransactionData
{
    double amount;
    string senderKey;
    string receiverKey;
    time_t timestamp;
};

//block class
class Block
{
  private:
    int index;
    size_t blockHash;
    size_t previousHash;
    size_t generateHash();

  public:
    Block(int idx, TransactionData d, size_t prevHash);

    //get original hash
    size_t getHash();

    //get previous hash
    size_t getPreviousHash();

    TransactionData data;

    //validate hash
    bool isHashValid();
};

//blockchain class

Block::Block(int idx, TransactionData d, size_t prevHash)
{
    index = idx;
    data = d;
    previousHash = prevHash;
    blockHash = generateHash();
}
//private
size_t Block::generateHash()
{
    hash<string> hash1;
    hash<size_t> hash2;
    hash<size_t> finalHash;
    string toHash = to_string(data.amount) + data.receiverKey + data.senderKey + to_string(data.timestamp);

    return finalHash(hash1(toHash) + hash2(previousHash));
};

//public
size_t Block::getHash()
{
    return blockHash;
}
size_t Block::getPreviousHash()
{
    return previousHash;
}

bool Block::isHashValid()
{
    return generateHash() == blockHash;
}

class Blockchain
{
  private:
    Block createGenesisBlock();

  public:
    //public chain
    vector<Block> chain;

    Blockchain();

    //Public Functions
    void addBlock(TransactionData data);
    bool isChainValid();

    Block *getLatestBlock();
};

Blockchain::Blockchain()
{
    Block genesis = createGenesisBlock();
    chain.push_back(genesis);
}

Block Blockchain::createGenesisBlock()
{
    time_t current;
    TransactionData d;
    d.amount = 0;
    d.receiverKey = "none";
    d.senderKey = "none";
    d.timestamp = time(&current);

    hash<int> hash1;
    Block genesis(0, d, hash1(0));
    return genesis;
}

// BAD TEST
Block *Blockchain::getLatestBlock()
{
    return &chain.back();
}

void Blockchain::addBlock(TransactionData d)
{
    int index = (int)chain.size() - 1;
    Block newBlock(index, d, getLatestBlock()->getHash());
}

bool Blockchain::isChainValid()
{
    vector<Block>::iterator it;
    int chainLen = (int)chain.size();

    for (it = chain.begin(); it != chain.end(); ++it)
    {
        Block currentBlock = *it;
        if (!currentBlock.isHashValid())
        {
            return false;
        }

        if (chainLen > 1)
        {
            Block previousBlock = *(it - 1);
            if (currentBlock.getPreviousHash() != previousBlock.getHash())
            {
                return false;
            }
        }
    }
}

int main()
{
    Blockchain GabsCoin;

    //first block
    TransactionData data1;
    time_t dataTime;
    data1.amount = 1.5;
    data1.receiverKey = "gabs";
    data1.senderKey = "ale";
    data1.timestamp = time(&dataTime);

    GabsCoin.addBlock(data1);

    cout << "is chain valid?" << endl
         << GabsCoin.isChainValid() << endl;

    return 0;
}

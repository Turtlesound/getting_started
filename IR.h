#ifndef IR_H
#define IR_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <fstream>
#include <sstream>
#include <iostream>

// Forward declarations
class Instruction;
class BasicBlock;
class CFG;

//  three-address instruction
class Instruction {
public:
    enum OpType {
        ASSIGN,     // x = y
        BINOP,      // x = y op z
        UNOP,       // x = op y
        PARAM,      // param x
        CALL,       // x = call f
        RETURN,     // return x
        ARRAY_STORE, // x[i] = y
        ARRAY_LOAD,  // x = y[i]
        GOTO,       // goto L
        IF_TRUE,    // if x goto L
        IF_FALSE,   // iffalse x goto L
        LABEL       // L:
    };
    
    OpType type;
    std::string result;
    std::string op1;
    std::string op2;
    std::string opcode;
    
    Instruction(OpType t, const std::string& res = "", const std::string& o1 = "", 
                const std::string& opc = "", const std::string& o2 = "")
        : type(t), result(res), op1(o1), opcode(opc), op2(o2) {}
    
    std::string toString() const {
        std::stringstream ss;
        switch(type) {
            case ASSIGN:
                ss << result << " = " << op1;
                break;
            case BINOP:
                ss << result << " = " << op1 << " " << opcode << " " << op2;
                break;
            case UNOP:
                ss << result << " = " << opcode << " " << op1;
                break;
            case PARAM:
                ss << "param " << result;
                break;
            case CALL:
                ss << result << " = call " << op1;
                break;
            case RETURN:
                ss << "return " << result;
                break;
            case ARRAY_STORE:
                ss << result << "[" << op1 << "] = " << op2;
                break;
            case ARRAY_LOAD:
                ss << result << " = " << op1 << "[" << op2 << "]";
                break;
            case GOTO:
                ss << "goto " << result;
                break;
            case IF_TRUE:
                ss << "if " << op1 << " goto " << result;
                break;
            case IF_FALSE:
                ss << "iffalse " << op1 << " goto " << result;
                break;
            case LABEL:
                ss << result << ":";
                break;
        }
        return ss.str();
    }
};

// Represents a basic block in the control flow graph
class BasicBlock {
public:
    std::string label;
    int blockId;
    std::vector<Instruction> instructions;
    std::vector<BasicBlock*> successors;
    std::vector<BasicBlock*> predecessors;
    
    // map for method call targets
    std::map<std::string, BasicBlock*> callTargets;
    
    explicit BasicBlock(const std::string& lbl, int id) : label(lbl), blockId(id) {}
    
    void addInstruction(const Instruction& instr) {
        instructions.push_back(instr);
    }
    
    void addSuccessor(BasicBlock* block) {
        if (block) {
            successors.push_back(block);
            block->predecessors.push_back(this);
        }
    }
    
    std::string toString() const {
        std::stringstream ss;
        for (const auto& instr : instructions) {
            ss << instr.toString() << "\\n";
        }
        return ss.str();
    }
};

// Represents a control flow graph
class CFG {
public:
    std::vector<std::unique_ptr<BasicBlock>> blocks;
    BasicBlock* entry;
    BasicBlock* exit;
    int tempCounter;
    int blockCounter;
    
    CFG() : entry(nullptr), exit(nullptr), tempCounter(0), blockCounter(0) {}
    
    // Delete copy constructor and copy assignment
    CFG(const CFG&) = delete;
    CFG& operator=(const CFG&) = delete;
    
    // move constructor and move assignment
    CFG(CFG&& other) noexcept 
        : blocks(std::move(other.blocks)), 
          entry(other.entry), 
          exit(other.exit),
          tempCounter(other.tempCounter),
          blockCounter(other.blockCounter) {
        other.entry = nullptr;
        other.exit = nullptr;
    }
    
    CFG& operator=(CFG&& other) noexcept {
        if (this != &other) {
            blocks = std::move(other.blocks);
            entry = other.entry;
            exit = other.exit;
            tempCounter = other.tempCounter;
            blockCounter = other.blockCounter;
            other.entry = nullptr;
            other.exit = nullptr;
        }
        return *this;
    }
    
    BasicBlock* createBlock() {
        int blockId = blockCounter++;
        std::string label = "block_" + std::to_string(blockId);
        blocks.push_back(std::make_unique<BasicBlock>(label, blockId));
        BasicBlock* block = blocks.back().get();
        
        // If first block, set as entry
        if (!entry) entry = block;
        
        return block;
    }
    
    std::string newTemp() {
        return "t" + std::to_string(tempCounter++);
    }
    
    void generateDotFile(const std::string& filename) const;
};

#endif // IR_H
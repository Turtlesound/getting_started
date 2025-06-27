#include "BytecodeInterpreter.h"
#include <sstream>
#include <algorithm>
#include <cctype>

BytecodeInterpreter::BytecodeInterpreter() : programCounter(0), running(false) {}

BytecodeInterpreter::~BytecodeInterpreter() {}

bool BytecodeInterpreter::loadBytecode(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open bytecode file: " << filename << std::endl;
        return false;
    }
    
    instructions.clear();
    labels.clear();
    methods.clear();
    
    std::string line;
    std::string currentClass = "";
    std::string currentMethod = "";
    bool inMethod = false;
    int instructionIndex = 0;
    
    if (verbose) {
        std::cout << "Loading bytecode from: " << filename << std::endl;
    }
    
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == ';') continue; // Skip empty lines and comments
        
        // Parse .class declarations
        if (line.find(".class") == 0) {
            size_t pos = line.find("public ");
            if (pos != std::string::npos) {
                currentClass = line.substr(pos + 7);
                if (verbose) std::cout << "Found class: " << currentClass << std::endl;
            }
            continue;
        }
        
        // Parse .method declarations
        if (line.find(".method") == 0) {
            size_t pos = line.find("public ");
            if (pos != std::string::npos) {
                std::string methodDecl = line.substr(pos + 7);
                size_t parenPos = methodDecl.find('(');
                if (parenPos != std::string::npos) {
                    currentMethod = methodDecl.substr(0, parenPos);
                    std::string fullMethodName = currentClass + "." + currentMethod;
                    
                    MethodInfo methodInfo;
                    methodInfo.className = currentClass;
                    methodInfo.methodName = currentMethod;
                    methodInfo.signature = methodDecl.substr(parenPos);
                    methodInfo.startAddress = instructionIndex;
                    
                    methods[fullMethodName] = methodInfo;
                    inMethod = true;
                    
                    if (verbose) std::cout << "Found method: " << fullMethodName << " at address " << instructionIndex << std::endl;
                }
            }
            continue;
        }
        
        // End of method
        if (line.find(".end method") == 0) {
            inMethod = false;
            continue;
        }
        
        // Skip other directives
        if (line[0] == '.') continue;
        
        // Process labels and instructions only if we're in a method
        if (inMethod) {
            if (isLabel(line)) {
                std::string label = extractLabel(line);
                labels[label] = instructionIndex;
                if (verbose) std::cout << "Label " << label << " at address " << instructionIndex << std::endl;
            } else {
                // Parse and store instruction
                BytecodeInstruction instr = parseInstruction(line);
                instructions.push_back(instr);
                
                if (verbose) {
                    std::cout << "Instruction " << instructionIndex << ": " << instr.opcode;
                    for (const auto& op : instr.operands) {
                        std::cout << " " << op;
                    }
                    std::cout << std::endl;
                }
                
                instructionIndex++;
            }
        }
    }
    
    file.close();
    
    if (verbose) {
        std::cout << "Loaded " << instructions.size() << " instructions" << std::endl;
        std::cout << "Found " << labels.size() << " labels" << std::endl;
        std::cout << "Found " << methods.size() << " methods" << std::endl;
    }
    
    return !instructions.empty();
}

void BytecodeInterpreter::execute() {
    if (instructions.empty()) {
        std::cerr << "No instructions to execute" << std::endl;
        return;
    }
    
    // Start execution from the main method or first instruction
    programCounter = 0;
    running = true;
    
    // Find and start from any main method if it exists
    auto mainIt = methods.end();
    for (const auto& method : methods) {
        if (method.first.find(".main") != std::string::npos) {
            mainIt = methods.find(method.first);
            break;
        }
    }
    
    if (mainIt != methods.end()) {
        programCounter = mainIt->second.startAddress;
        if (verbose) std::cout << "Starting execution from " << mainIt->first << " at address " << programCounter << std::endl;
    }
    
    while (running && programCounter < static_cast<int>(instructions.size())) {
        BytecodeInstruction& instr = instructions[programCounter];
        
        if (verbose) {
            printDebugInfo(instr);
        }
        
        executeInstruction(instr);
        
        // Move to next instruction (unless it was a jump)
        if (running) {
            programCounter++;
        }
    }
    
    if (verbose) {
        std::cout << "Execution finished." << std::endl;
    }
}

void BytecodeInterpreter::executeInstruction(const BytecodeInstruction& instr) {
    const std::string& op = instr.opcode;
    
    if (op == "iload") {
        if (!instr.operands.empty()) {
            executeILoad(stringToInt(instr.operands[0]));
        }
    }
    else if (op == "iconst") {
        if (!instr.operands.empty()) {
            executeIConst(stringToInt(instr.operands[0]));
        }
    }
    else if (op == "istore") {
        if (!instr.operands.empty()) {
            executeIStore(stringToInt(instr.operands[0]));
        }
    }
    else if (op == "iadd") {
        executeIAdd();
    }
    else if (op == "isub") {
        executeISub();
    }
    else if (op == "imul") {
        executeIMul();
    }
    else if (op == "idiv") {
        executeIDiv();
    }
    else if (op == "ilt") {
        executeILt();
    }
    else if (op == "igt") {
        executeIGt();
    }
    else if (op == "ieq") {
        executeIEq();
    }
    else if (op == "iand") {
        executeIAnd();
    }
    else if (op == "ior") {
        executeIOr();
    }
    else if (op == "inot") {
        executeINot();
    }
    else if (op == "goto") {
        if (!instr.operands.empty()) {
            executeGoto(instr.operands[0]);
        }
    }
    else if (op == "iffalse") {
        if (instr.operands.size() >= 2 && instr.operands[0] == "goto") {
            executeIfFalse(instr.operands[1]);
        }
    }
    else if (op == "invokevirtual") {
        if (!instr.operands.empty()) {
            executeInvokeVirtual(instr.operands[0]);
        }
    }
    else if (op == "ireturn") {
        executeIReturn();
    }
    else if (op == "print") {
        executePrint();
    }
    else if (op == "stop") {
        executeStop();
    }
    else if (op == "new" || op == "dup" || op == "invokespecial" || op == "aload_0" || op == "return" || op == "invokestatic" || op == "pop") {
        // Handle object-related and method-related instructions
        if (op == "new") {
            // For new, just push a dummy object reference (1)
            dataStack.push(1);
        } else if (op == "return") {
            // Simple return without value
            running = false;
        } else if (op == "invokestatic") {
            // Static method call - for now just continue
            if (verbose) std::cout << "  invokestatic (ignored)" << std::endl;
        } else if (op == "pop") {
            // Pop value from stack
            if (!dataStack.empty()) {
                dataStack.pop();
            }
        }
        // dup, invokespecial, aload_0 are handled but don't affect our simple execution
    }
    else {
        std::cerr << "Warning: Unknown instruction: " << op << std::endl;
    }
}

// Stack operations implementation
void BytecodeInterpreter::executeILoad(int index) {
    int value = getCurrentLocalVar(index);
    dataStack.push(value);
    if (verbose) std::cout << "  iload " << index << " -> pushed " << value << std::endl;
}

void BytecodeInterpreter::executeIConst(int value) {
    dataStack.push(value);
    if (verbose) std::cout << "  iconst " << value << " -> pushed " << value << std::endl;
}

void BytecodeInterpreter::executeIStore(int index) {
    if (dataStack.empty()) {
        std::cerr << "Error: Stack underflow in istore" << std::endl;
        running = false;
        return;
    }
    
    int value = dataStack.top();
    dataStack.pop();
    setCurrentLocalVar(index, value);
    if (verbose) std::cout << "  istore " << index << " -> stored " << value << std::endl;
}

// Arithmetic operations
void BytecodeInterpreter::executeIAdd() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in iadd" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = v2 + v1;  // Note: v2 + v1 (stack order)
    dataStack.push(result);
    if (verbose) std::cout << "  iadd: " << v2 << " + " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeISub() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in isub" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = v2 - v1;  // Note: v2 - v1 (stack order)
    dataStack.push(result);
    if (verbose) std::cout << "  isub: " << v2 << " - " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeIMul() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in imul" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = v2 * v1;
    dataStack.push(result);
    if (verbose) std::cout << "  imul: " << v2 << " * " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeIDiv() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in idiv" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    
    if (v1 == 0) {
        std::cerr << "Error: Division by zero" << std::endl;
        running = false;
        return;
    }
    
    int result = v2 / v1;
    dataStack.push(result);
    if (verbose) std::cout << "  idiv: " << v2 << " / " << v1 << " = " << result << std::endl;
}

// Comparison operations
void BytecodeInterpreter::executeILt() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in ilt" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = (v2 < v1) ? 1 : 0;
    dataStack.push(result);
    if (verbose) std::cout << "  ilt: " << v2 << " < " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeIGt() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in igt" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = (v2 > v1) ? 1 : 0;
    dataStack.push(result);
    if (verbose) std::cout << "  igt: " << v2 << " > " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeIEq() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in ieq" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = (v2 == v1) ? 1 : 0;
    dataStack.push(result);
    if (verbose) std::cout << "  ieq: " << v2 << " == " << v1 << " = " << result << std::endl;
}

// Logical operations
void BytecodeInterpreter::executeIAnd() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in iand" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = (v1 * v2 == 0) ? 0 : 1;  // As per Table 2 specification
    dataStack.push(result);
    if (verbose) std::cout << "  iand: " << v2 << " && " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeIOr() {
    if (dataStack.size() < 2) {
        std::cerr << "Error: Stack underflow in ior" << std::endl;
        running = false;
        return;
    }
    
    int v1 = dataStack.top(); dataStack.pop();
    int v2 = dataStack.top(); dataStack.pop();
    int result = (v1 + v2 == 0) ? 0 : 1;  // As per Table 2 specification
    dataStack.push(result);
    if (verbose) std::cout << "  ior: " << v2 << " || " << v1 << " = " << result << std::endl;
}

void BytecodeInterpreter::executeINot() {
    if (dataStack.empty()) {
        std::cerr << "Error: Stack underflow in inot" << std::endl;
        running = false;
        return;
    }
    
    int v = dataStack.top(); dataStack.pop();
    int result = (v == 0) ? 1 : 0;
    dataStack.push(result);
    if (verbose) std::cout << "  inot: !" << v << " = " << result << std::endl;
}

// Control flow operations
void BytecodeInterpreter::executeGoto(const std::string& label) {
    auto it = labels.find(label);
    if (it != labels.end()) {
        programCounter = it->second - 1; // -1 because it will be incremented
        if (verbose) std::cout << "  goto " << label << " -> jumping to address " << it->second << std::endl;
    } else {
        std::cerr << "Error: Label not found: " << label << std::endl;
        running = false;
    }
}

void BytecodeInterpreter::executeIfFalse(const std::string& label) {
    if (dataStack.empty()) {
        std::cerr << "Error: Stack underflow in iffalse" << std::endl;
        running = false;
        return;
    }
    
    int v = dataStack.top(); dataStack.pop();
    if (v == 0) {  // If false (0), jump
        auto it = labels.find(label);
        if (it != labels.end()) {
            programCounter = it->second - 1; // -1 because it will be incremented
            if (verbose) std::cout << "  iffalse " << label << " -> condition false, jumping to address " << it->second << std::endl;
        } else {
            std::cerr << "Error: Label not found: " << label << std::endl;
            running = false;
        }
    } else {
        if (verbose) std::cout << "  iffalse " << label << " -> condition true, continuing" << std::endl;
    }
}

// Method operations
void BytecodeInterpreter::executeInvokeVirtual(const std::string& methodName) {
    if (verbose) std::cout << "  invokevirtual " << methodName << std::endl;
    
    // Extract method name from methodName (e.g., "t1.calcSum" -> "calcSum")
    std::string actualMethodName = methodName;
    size_t dotPos = methodName.find('.');
    if (dotPos != std::string::npos) {
        actualMethodName = methodName.substr(dotPos + 1);
    }
    
    // Find the method in our method table
    std::string fullMethodName = "";
    for (const auto& method : methods) {
        if (method.second.methodName == actualMethodName) {
            fullMethodName = method.first;
            break;
        }
    }
    
    if (fullMethodName.empty()) {
        // Method not found, use default behavior for system methods
        if (actualMethodName == "println" || methodName.find("System.out.println") != std::string::npos) {
            // Handle println - just ignore for now
            if (!dataStack.empty()) {
                dataStack.pop(); // Pop the parameter
            }
        } else {
            // For other unknown methods, just push 0
            if (!dataStack.empty()) {
                dataStack.pop(); // Pop object reference if present
            }
            dataStack.push(0);
        }
        return;
    }
    
    // Get method info
    const MethodInfo& methodInfo = methods[fullMethodName];
    
    // Check for recursion depth (stack overflow protection)
    if (activationStack.size() >= MAX_RECURSION_DEPTH) {
        std::cerr << "Error: Stack overflow - maximum recursion depth exceeded" << std::endl;
        running = false;
        return;
    }
    
    // Create activation record
    ActivationRecord record;
    record.methodName = fullMethodName;
    record.returnAddress = programCounter + 1; // Return to next instruction
    
    // Check if method has parameters (indicated by presence of '(' and something before ')')
    bool hasParameters = false;
    if (methodInfo.signature.length() > 2) { // More than just "()"
        std::string sigContent = methodInfo.signature.substr(1, methodInfo.signature.find(')') - 1);
        hasParameters = !sigContent.empty();
    }
    
    if (hasParameters && !dataStack.empty()) {
        // Pop parameter from stack and store in local variable 1
        int param = dataStack.top(); 
        dataStack.pop();
        record.localVariables[1] = param;
        
        if (verbose) std::cout << "  Method call " << fullMethodName << " with param=" << param << std::endl;
    } else if (verbose) {
        std::cout << "  Method call " << fullMethodName << " (no parameters)" << std::endl;
    }
    
    // Push activation record
    activationStack.push(record);
    
    // Jump to method start
    programCounter = methodInfo.startAddress - 1; // -1 because it will be incremented
    
    if (verbose) std::cout << "  Jumping to method " << fullMethodName << " at address " << methodInfo.startAddress << std::endl;
}

void BytecodeInterpreter::executeIReturn() {
    // The return value should be on top of the data stack
    int returnValue = 0;
    if (!dataStack.empty()) {
        returnValue = dataStack.top();
        dataStack.pop();
    }
    
    if (!activationStack.empty()) {
        // Pop activation record and restore context
        ActivationRecord record = activationStack.top();
        activationStack.pop();
        programCounter = record.returnAddress - 1; // -1 because it will be incremented by main loop
        
        // Push the return value back onto the stack
        dataStack.push(returnValue);
        
        if (verbose) std::cout << "  ireturn -> returning value " << returnValue << " to address " << record.returnAddress << std::endl;
    } else {
        // End of main method
        running = false;
        if (verbose) std::cout << "  ireturn -> program finished with value " << returnValue << std::endl;
    }
}

// I/O operations
void BytecodeInterpreter::executePrint() {
    if (dataStack.empty()) {
        std::cerr << "Error: Stack underflow in print" << std::endl;
        running = false;
        return;
    }
    
    int value = dataStack.top(); dataStack.pop();
    std::cout << value << std::endl;
    if (verbose) std::cout << "  print -> output: " << value << std::endl;
}

void BytecodeInterpreter::executeStop() {
    running = false;
    if (verbose) std::cout << "  stop -> execution stopped" << std::endl;
}

// Helper methods
int BytecodeInterpreter::getCurrentLocalVar(int index) {
    if (!activationStack.empty()) {
        auto& vars = activationStack.top().localVariables;
        auto it = vars.find(index);
        return (it != vars.end()) ? it->second : 0;
    }
    // Use global local variable store for main method
    auto it = globalLocalVars.find(index);
    return (it != globalLocalVars.end()) ? it->second : 0;
}

void BytecodeInterpreter::setCurrentLocalVar(int index, int value) {
    if (!activationStack.empty()) {
        activationStack.top().localVariables[index] = value;
    } else {
        // Use global local variable store for main method
        globalLocalVars[index] = value;
    }
}

void BytecodeInterpreter::printDebugInfo(const BytecodeInstruction& instr) {
    std::cout << "PC=" << programCounter << " Stack[";
    std::stack<int> temp = dataStack;
    std::vector<int> elements;
    while (!temp.empty()) {
        elements.push_back(temp.top());
        temp.pop();
    }
    for (int i = elements.size() - 1; i >= 0; i--) {
        std::cout << elements[i];
        if (i > 0) std::cout << ",";
    }
    std::cout << "] Execute: " << instr.opcode;
    for (const auto& op : instr.operands) {
        std::cout << " " << op;
    }
    std::cout << std::endl;
}

// Parsing helper methods
bool BytecodeInterpreter::isLabel(const std::string& line) {
    return !line.empty() && line.back() == ':';
}

std::string BytecodeInterpreter::extractLabel(const std::string& line) {
    if (isLabel(line)) {
        return line.substr(0, line.length() - 1);
    }
    return "";
}

BytecodeInstruction BytecodeInterpreter::parseInstruction(const std::string& line) {
    std::vector<std::string> tokens = tokenize(line);
    if (tokens.empty()) {
        return BytecodeInstruction("");
    }
    
    BytecodeInstruction instr(tokens[0]);
    for (size_t i = 1; i < tokens.size(); i++) {
        instr.operands.push_back(tokens[i]);
    }
    
    return instr;
}

std::vector<std::string> BytecodeInterpreter::tokenize(const std::string& str) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;
    
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    return tokens;
}

std::string BytecodeInterpreter::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

bool BytecodeInterpreter::isNumeric(const std::string& str) {
    if (str.empty()) return false;
    
    size_t start = (str[0] == '-') ? 1 : 0;
    for (size_t i = start; i < str.length(); i++) {
        if (!std::isdigit(str[i])) return false;
    }
    return true;
}

int BytecodeInterpreter::stringToInt(const std::string& str) {
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        return 0;
    }
}

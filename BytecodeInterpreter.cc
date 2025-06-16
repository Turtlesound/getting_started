#include "BytecodeInterpreter.h"

BytecodeInterpreter::BytecodeInterpreter() {
}

bool BytecodeInterpreter::loadBytecode(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open bytecode file: " << filename << std::endl;
        return false;
    }
    
    std::shared_ptr<Class> currentClass = nullptr;
    std::shared_ptr<Method> currentMethod = nullptr;
    
    std::string line;
    while (std::getline(file, line)) {
        // Trim whitespace
        line.erase(0, line.find_first_not_of(" \t"));
        if (line.size() > 0) 
            line.erase(line.find_last_not_of(" \t") + 1);
        
        if (line.empty() || line[0] == ';') {
            continue;  // Skip empty lines and comments
        }
        
        if (line.substr(0, 7) == ".class ") {
            // Parse class definition
            currentClass = std::make_shared<Class>();
            std::string classDecl = line.substr(7);
            
            size_t spacePos = classDecl.find(' ');
            if (spacePos != std::string::npos) {
                currentClass->name = classDecl.substr(spacePos + 1);
            } else {
                currentClass->name = classDecl;
            }
            
            // Store first class seen as potential main class
            if (mainClassName.empty()) {
                mainClassName = currentClass->name;
            }
            
            // Store class as we create it - don't wait until end of file
            classes[currentClass->name] = currentClass;
            if (verbose) {
                std::cout << "DEBUG: Created class: " << currentClass->name << std::endl;
            }
        }
        else if (line.substr(0, 7) == ".super ") {
            // Parse superclass
            if (currentClass) {
                currentClass->superclass = line.substr(7);
            }
        }
        else if (line.substr(0, 8) == ".method ") {
            // Parse method definition
            currentMethod = std::make_shared<Method>();
            std::string methodDecl = line.substr(8);
            
            // Extract method name and signature
            size_t parenPos = methodDecl.find('(');
            if (parenPos != std::string::npos) {
                size_t spacePos = methodDecl.find(' ');
                if (spacePos != std::string::npos && spacePos < parenPos) {
                    currentMethod->name = methodDecl.substr(spacePos + 1, parenPos - spacePos - 1);
                } else {
                    currentMethod->name = methodDecl.substr(0, parenPos);
                }
                currentMethod->signature = methodDecl.substr(parenPos);
            } else {
                currentMethod->name = methodDecl;
                currentMethod->signature = "()V";  // Default void return
            }
            
            currentMethod->maxLocals = 1;  // Reserve space for "this"
        }
        else if (line == ".end method") {
            // End of method definition
            if (currentClass && currentMethod) {
                currentClass->methods[currentMethod->name] = currentMethod;
                currentMethod = nullptr;
            }
        }
        else if (line.back() == ':') {
            // Label definition
            if (currentMethod) {
                std::string label = line.substr(0, line.size() - 1);
                currentMethod->labels[label] = currentMethod->instructions.size();
            }
        }
        else if (currentMethod) {
            // ANY line that's not a directive and we're in a method is an instruction
            // Track max locals for frame size
            if (line.substr(0, 7) == "istore ") {
                std::istringstream ss(line.substr(7));
                int index;
                if (ss >> index) {
                    currentMethod->maxLocals = std::max(currentMethod->maxLocals, index + 1);
                }
            }
            
            currentMethod->instructions.push_back(line);
        }
    }
    
    // Store last class if there was one
    if (currentClass) {
        classes[currentClass->name] = currentClass;
    }
    
    file.close();
    return !classes.empty();
}

void BytecodeInterpreter::execute() {
    if (classes.empty()) {
        std::cerr << "No classes loaded" << std::endl;
        return;
    }
    
    if (verbose) {
        std::cout << "DEBUG: Loaded classes: ";
        for (const auto& [name, _] : classes) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    
    // Try to find main method in D1 class first (common convention)
    if (classes.find("D1") != classes.end() && classes["D1"]->methods.find("main") != classes["D1"]->methods.end()) {
        if (verbose) {
            std::cout << "Starting execution from D1.main" << std::endl;
        }
        std::cout.flush();
        executeMethod("D1", "main");
        return;
    }
    
    // Try to find main method in any class
    for (const auto& [className, classObj] : classes) {
        if (verbose) {
            std::cout << "DEBUG: Found class: " << className << std::endl;
            for (const auto& [methodName, method] : classObj->methods) {
                std::cout << "DEBUG:   Method: " << methodName << std::endl;
            }
        }
        
        if (classObj->methods.find("main") != classObj->methods.end()) {
            if (verbose) {
                std::cout << "Starting execution from " << className << ".main" << std::endl;
            }
            std::cout.flush();
            executeMethod(className, "main");
            return;
        }
    }
    
    // If all else fails, try to find a static main method
    for (const auto& [className, classObj] : classes) {
        if (classObj->methods.find("main([Ljava/lang/String;)V") != classObj->methods.end()) {
            if (verbose) {
                std::cout << "Starting execution from static " << className << ".main" << std::endl;
            }
            std::cout.flush();
            executeMethod(className, "main([Ljava/lang/String;)V");
            return;
        }
    }
    
    std::cerr << "No main method found" << std::endl;
}

void BytecodeInterpreter::executeMethod(const std::string& className, const std::string& methodName, int objRef, const std::vector<int>& params) {
    if (verbose) {
        std::cout << "DEBUG: Executing method " << className << "." << methodName;
        if (objRef != -1) std::cout << " on objRef " << objRef;
        std::cout << " with " << params.size() << " params." << std::endl;
        std::cout.flush();
    }
    
    auto classIt = classes.find(className);
    if (classIt == classes.end()) {
        std::cerr << "Class not found: " << className << std::endl;
        return;
    }
    
    auto methodIt = classIt->second->methods.find(methodName);
    if (methodIt == classIt->second->methods.end()) {
        std::cerr << "Method not found: " << className << "." << methodName << std::endl;
        return;
    }
    
    auto method = methodIt->second;
    
    Frame frame;
    frame.methodName = methodName; 
    frame.className = className;
    // frame.returnPC is set by caller (e.g. invokevirtual) if it's a call
    if (!activationStack.empty()) { // If called from another method
         // pc is current instruction pointer of caller. Return PC should be next instruction.
         
    } else {
        frame.returnPC = -1; // Entry method
    }

    frame.locals.resize(std::max(method->maxLocals, 10), 0); 

    if (objRef != -1) { // If it's an instance method call with an object reference
        frame.locals[0] = objRef; // 'this'
    }
    for (size_t i = 0; i < params.size(); i++) {
        if ((i + 1) < frame.locals.size()) { // Ensure space for parameters
            frame.locals[i + 1] = params[i];
        } else {
            std::cerr << "Error: Not enough local variable space for parameter " << i << std::endl;
        }
    }
    
    activationStack.push_back(frame);
    
    int pc = 0;
    while (pc >= 0 && pc < method->instructions.size()) {
        std::string instr = method->instructions[pc];
        int prevPC = pc; // Store pc before executing instruction
        executeInstruction(instr, pc);
        
        if (pc == -100) { // Special signal for method return handled by ireturn
            break; 
        }
       
        if (pc == prevPC) {
            pc++;
        }
    }
    
   
    if (!activationStack.empty() && activationStack.back().methodName == methodName && activationStack.back().className == className) {
        if (verbose) std::cout << "DEBUG: Method " << className << "." << methodName << " ended without explicit ireturn. Popping frame." << std::endl;
        activationStack.pop_back();

        if (!method->signature.empty() && method->signature.back() == 'I') {
             if (verbose) std::cout << "DEBUG: Pushing default return value 0 for " << className << "." << methodName << std::endl;
             push(0);
        }
    }
}


void BytecodeInterpreter::executeMethod(const std::string& className, const std::string& methodName) {

    int objRef = -1;
    if (className == "D1" && methodName == "main") { // Trying to see if i can solve d1.
        objRef = allocateObject(className); 
        if (verbose) std::cout << "DEBUG: Allocated dummy objRef " << objRef << " for " << className << "." << methodName << std::endl;
    }
    executeMethod(className, methodName, objRef, {});
}

void BytecodeInterpreter::printStackState() {
    std::cout << "Data stack: [";
    for (size_t i = 0; i < dataStack.size(); i++) {
        if (i > 0) std::cout << ", ";
        std::cout << dataStack[i];
    }
    std::cout << "]" << std::endl;
}

void BytecodeInterpreter::executeInstruction(const std::string& instr, int& pc) {
    std::istringstream iss(instr);
    std::string opcode;
    iss >> opcode;
    
    // Only print instruction debug info with -v flag
    if (verbose) {
        std::cout << "DEBUG: Executing instruction: " << instr << std::endl;
        std::cout.flush();
        
        std::cout << "\tStack before: ";
        printStackState();
        std::cout.flush();
    }
    
    if (opcode == "iconst") {
        std::string value;
        iss >> value;
        
        if (value == "true") {
            push(1);
        } else if (value == "false") {
            push(0);
        } else {
            push(std::stoi(value));
        }
    }
    else if (opcode == "iload") {
        int index;
        iss >> index;
        
        if (!activationStack.empty() && index < activationStack.back().locals.size()) {
            push(activationStack.back().locals[index]);
        } else {
            std::cerr << "Invalid local variable index: " << index << std::endl;
            push(0);
        }
    }
    else if (opcode == "istore") {
        int index;
        iss >> index;
        
        if (!activationStack.empty() && index < activationStack.back().locals.size()) {
            activationStack.back().locals[index] = pop();
        } else {
            std::cerr << "Invalid local variable index: " << index << std::endl;
            pop();  // Discard value
        }
    }
    else if (opcode == "iadd") {
        int b = pop();
        int a = pop();
        push(a + b);
    }
    else if (opcode == "isub") {
        int b = pop();
        int a = pop();
        push(a - b);
    }
    else if (opcode == "imul") {
        int b = pop();
        int a = pop();
        push(a * b);
    }
    else if (opcode == "idiv") {
        int b = pop();
        int a = pop();
        if (b == 0) {
            std::cerr << "Division by zero" << std::endl;
            push(0);
        } else {
            push(a / b);
        }
    }
    else if (opcode == "ilt") {
        int b = pop();
        int a = pop();
        push(a < b ? 1 : 0);
    }
    else if (opcode == "igt") {
        int b = pop();
        int a = pop();
        push(a > b ? 1 : 0);
    }
    else if (opcode == "ieq") {
        int b = pop();
        int a = pop();
        push(a == b ? 1 : 0);
    }
    else if (opcode == "iand") {
        int b = pop();
        int a = pop();
        push(a && b);
    }
    else if (opcode == "ior") {
        int b = pop();
        int a = pop();
        push(a || b);
    }
    else if (opcode == "inot") {
        int a = pop();
        push(!a);
    }
    else if (opcode == "ineg") {
        int a = pop();
        push(-a);
    }
    else if (opcode == "print") {
        int value = pop();
        std::cout << value << std::endl;
    }
    else if (opcode == "newarray") {
        int size = pop();
        int arrayRef = allocateArray(size);
        push(arrayRef);
    }
    else if (opcode == "new") {
        std::string className;
        iss >> className;
        int objRef = allocateObject(className);
        push(objRef);
    }
    else if (opcode == "arraylength") {
        int arrayRef = pop();
        
        if (arrayRef < 0 || arrayRef >= arrays.size() || !arrays[arrayRef]) {
            std::cerr << "Invalid array reference: " << arrayRef << std::endl;
            push(0);
        } else {
            push(arrays[arrayRef]->elements.size());
        }
    }
    else if (opcode == "iaload") {
        int index = pop();
        int arrayRef = pop();
        
        if (arrayRef < 0 || arrayRef >= arrays.size() || !arrays[arrayRef]) {
            std::cerr << "Invalid array reference: " << arrayRef << std::endl;
            push(0);
        } else {
            auto array = arrays[arrayRef];
            if (index < 0 || index >= array->elements.size()) {
                std::cerr << "Array index out of bounds: " << index << std::endl;
                push(0);
            } else {
                push(array->elements[index]);
            }
        }
    }
    else if (opcode == "iastore") {
        int value = pop();
        int index = pop();
        int arrayRef = pop();
        
        if (arrayRef < 0 || arrayRef >= arrays.size() || !arrays[arrayRef]) {
            std::cerr << "Invalid array reference: " << arrayRef << std::endl;
        } else {
            auto array = arrays[arrayRef];
            if (index < 0 || index >= array->elements.size()) {
                std::cerr << "Array index out of bounds: " << index << std::endl;
            } else {
                array->elements[index] = value;
            }
        }
    }
    else if (opcode == "dup") {
        // Implement dup instruction - duplicate top stack value
        if (!dataStack.empty()) {
            int value = peek();
            push(value);
        } else {
            std::cerr << "Cannot dup from empty stack" << std::endl;
        }
    }
        else if (opcode == "invokevirtual") {
        std::string methodNameWithSig;
        iss >> methodNameWithSig;
        
        size_t sigParamStart = methodNameWithSig.find('(');
        size_t sigParamEnd = methodNameWithSig.find(')');
        std::string actualMethodName = methodNameWithSig.substr(0, sigParamStart);
        std::string signatureString = methodNameWithSig.substr(sigParamStart); // Includes parens and return type
        
        int numParamsToPop = 0;
        if (sigParamStart != std::string::npos && sigParamEnd != std::string::npos && sigParamEnd > sigParamStart) {
            std::string paramTypes = methodNameWithSig.substr(sigParamStart + 1, sigParamEnd - (sigParamStart + 1));
            for (char c : paramTypes) {
                if (c == 'I') numParamsToPop++; // Count 'I's for int params
            }
        }

        if (actualMethodName == "out.println") {
            // Assumes (I)I or (I)V signature, so numParamsToPop should be 1
            if (numParamsToPop == 1 && !dataStack.empty()) {
                int value = pop();
                std::cout << value << std::endl;
            } else if (numParamsToPop == 0 && !dataStack.empty() && methodNameWithSig == "out.println()I" ) {

                 int value = pop(); // if it was meant to consume one.
                 std::cout << value << std::endl;
            }
             else {
                std::cerr << "Stack underflow or incorrect signature for out.println" << std::endl;
            }

            return; // pc will be incremented by caller loop
        }
        
        std::vector<int> call_params;
        for (int i = 0; i < numParamsToPop; i++) {
            if (dataStack.empty()) {
                std::cerr << "Stack underflow popping parameters for " << actualMethodName << std::endl;
                // Fill with defaults or error out
                for(int k=0; k < numParamsToPop - i; ++k) call_params.insert(call_params.begin(), 0);
                break;
            }
            call_params.insert(call_params.begin(), pop()); // Pop in correct order for callee
        }
        
        if (dataStack.empty()) {
             std::cerr << "Stack underflow: Missing object reference for " << actualMethodName << std::endl;
             return; // pc will be incremented
        }
        int objRef = pop();
        
        if (objRef >= 0 && objRef < objects.size() && objects[objRef]) {
            std::string targetClassName = objects[objRef]->className;
            
            if (verbose) {
                std::cout << "DEBUG: Invoking " << targetClassName << "." << actualMethodName << signatureString 
                          << " on objRef " << objRef << " with " << call_params.size() << " params." << std::endl;
            }

            // Store return PC for the *current* frame before calling new method
            if (!activationStack.empty()) {
                activationStack.back().returnPC = pc + 1; // Next instruction in current method
            }
            
            executeMethod(targetClassName, actualMethodName /* use full name with sig if methods are stored that way */, objRef, call_params);
            

        } else {
            std::cerr << "Invalid object reference: " << objRef << " for method " << actualMethodName << std::endl;
        }

    } else if (opcode == "invokespecial") {
        std::string methodNameFull;
        iss >> methodNameFull;
        
        std::string targetClass;
        std::string methodName;
        
        size_t slashPos = methodNameFull.find('/');
        if (slashPos != std::string::npos) {
            targetClass = methodNameFull.substr(0, slashPos);
            methodName = methodNameFull.substr(slashPos + 1);
            
            // Handle constructor calls
            if (methodName == "<init>()V") {
                // Constructor call - just pop the object reference
                int objRef = pop();
                if (verbose) {
                    std::cout << "DEBUG: Constructor call to " << targetClass << ".<init> on object " << objRef << std::endl;
                }
                // No need to actually call anything for the simple interpreter
            } else {
                // Regular special method call
                activationStack.back().returnPC = pc;
                executeMethod(targetClass, methodName);
                pc = activationStack.back().returnPC;
            }
        }
    }
    else if (opcode == "goto") {
        std::string labelName;
        iss >> labelName;
        
        if (!activationStack.empty()) {
            auto& frame = activationStack.back();
            auto classIt = classes.find(frame.className);
            if (classIt != classes.end()) {
                auto methodIt = classIt->second->methods.find(frame.methodName);
                if (methodIt != classIt->second->methods.end()) {
                    int targetPC = resolveLabel(labelName, methodIt->second);
                    if (targetPC >= 0) {
                        pc = targetPC - 1;  // Subtract 1 because PC will be incremented after instruction
                    }
                }
            }
        }
    }
    else if (opcode == "ifne") {
        std::string labelName;
        iss >> labelName;
        
        int condition = pop();
        if (condition != 0) {
            // Condition true, jump to label
            if (!activationStack.empty()) {
                auto& frame = activationStack.back();
                auto classIt = classes.find(frame.className);
                if (classIt != classes.end()) {
                    auto methodIt = classIt->second->methods.find(frame.methodName);
                    if (methodIt != classIt->second->methods.end()) {
                        int targetPC = resolveLabel(labelName, methodIt->second);
                        if (targetPC >= 0) {
                            pc = targetPC - 1;  // Subtract 1 because PC will be incremented after instruction
                        }
                    }
                }
            }
        }
    }
    else if (opcode == "ifeq") {
        std::string labelName;
        iss >> labelName;
        
        int condition = pop();
        if (condition == 0) {
            // Condition false, jump to label
            if (!activationStack.empty()) {
                auto& frame = activationStack.back();
                auto classIt = classes.find(frame.className);
                if (classIt != classes.end()) {
                    auto methodIt = classIt->second->methods.find(frame.methodName);
                    if (methodIt != classIt->second->methods.end()) {
                        int targetPC = resolveLabel(labelName, methodIt->second);
                        if (targetPC >= 0) {
                            pc = targetPC - 1;  // Subtract 1 because PC will be incremented after instruction
                        }
                    }
                }
            }
        }
    }
    else if (opcode == "ireturn") {
        int returnValue = 0;
        if (!dataStack.empty()) { // Method might be void or return value is optional on stack
            // Check method signature to see if it should return a value
            // For now, assume if stack is not empty, top is return value for non-void.
            returnValue = pop();
        } else if (!activationStack.empty()) {
            // If stack is empty, but method is supposed to return (e.g. ()I),returnValue remains 0.
            auto currentMethodForReturn = classes[activationStack.back().className]->methods[activationStack.back().methodName];
            if (currentMethodForReturn && !currentMethodForReturn->signature.empty() && currentMethodForReturn->signature.back() != 'V') {
                // It's a non-void method, but stack was empty. returnValue is 0.
            } else {
                // Void method, or issue.
            }
        }


        if (activationStack.empty()) {
            std::cerr << "Return with no active frame" << std::endl;
            pc = -1; // Terminate execution
            return;
        }
        
        int retPC = activationStack.back().returnPC;
        activationStack.pop_back();
        
        // Push return value onto the caller's stack
        // (Only if the original method was not void - check signature)
        // For now, always push if a value was popped or is default 0 for non-void.
        push(returnValue); 
        
        if (verbose) {
            std::cout << "DEBUG: Returning value: " << returnValue << ". Jumping to caller's PC: " << retPC << std::endl;
        }
        
        if (retPC != -1 && !activationStack.empty()) { // If there's a caller to return to
             // The pc of the *caller* needs to be set.
             // This is tricky. The current 'pc' is for the *callee*.
             // The outer loop in executeMethod handles pc for the *current* method.
             // When ireturn happens, it should signal to pop frame and restore caller's pc.
             // The simplest is to have executeInstruction change 'pc' to a special value
             // that the executeMethod's loop interprets as "method has returned".
             pc = -100; // Special value indicating method return
        } else {
             pc = -1; // No caller (e.g. main returned), terminate.
        }
    }
    else {
        std::cerr << "Unknown bytecode instruction: " << opcode << std::endl;
    }
    
    if (verbose) {
        std::cout << "\tStack after:  ";
        printStackState();
        std::cout << std::endl;
        std::cout.flush();
    }
}

void BytecodeInterpreter::push(int value) {
    dataStack.push_back(value);
}

int BytecodeInterpreter::pop() {
    if (dataStack.empty()) {
        std::cerr << "Data stack underflow" << std::endl;
        return 0;
    }
    
    int value = dataStack.back();
    dataStack.pop_back();
    return value;
}

int BytecodeInterpreter::peek() {
    if (dataStack.empty()) {
        std::cerr << "Data stack underflow on peek" << std::endl;
        return 0;
    }
    
    return dataStack.back();
}

int BytecodeInterpreter::resolveLabel(const std::string& label, const std::shared_ptr<Method>& method) {
    // Labels can be in format L1 or block_1 - handle both
    auto labelIt = method->labels.find(label);
    if (labelIt != method->labels.end()) {
        return labelIt->second;
    }
    
    // Try converting block_X to LX
    if (label.substr(0, 6) == "block_") {
        std::string altLabel = "L" + label.substr(6);
        labelIt = method->labels.find(altLabel);
        if (labelIt != method->labels.end()) {
            return labelIt->second;
        }
    }
    
    // Try looking up Ln label
    if (label.substr(0, 1) == "L") {
        labelIt = method->labels.find(label);
        if (labelIt != method->labels.end()) {
            return labelIt->second;
        }
    }
    
    std::cerr << "Label not found: " << label << std::endl;
    return -1;
}

int BytecodeInterpreter::allocateArray(int size) {
    if (size < 0) {
        std::cerr << "Cannot create array with negative length: " << size << std::endl;
        return -1;
    }
    
    auto array = std::make_shared<Array>();
    array->elements.resize(size, 0);  // Initialize all elements to 0
    
    // Find an empty slot or add to the end
    for (size_t i = 0; i < arrays.size(); i++) {
        if (!arrays[i]) {
            arrays[i] = array;
            return i;
        }
    }
    
    arrays.push_back(array);
    return arrays.size() - 1;
}

int BytecodeInterpreter::allocateObject(const std::string& className) {
    auto object = std::make_shared<Object>();
    object->className = className;
    
    // Find an empty slot or add to the end
    for (size_t i = 0; i < objects.size(); i++) {
        if (!objects[i]) {
            objects[i] = object;
            return i;
        }
    }
    
    objects.push_back(object);
    return objects.size() - 1;
}
#include <iostream>
#include "string"
using namespace std;


void menu() {
    cout << "1 - From Infix to Prefix and Postfix \n";
    cout << "2 - Checking the expression for errors \n";
    cout << "3 - Calculate expression \n"; 
}


struct ListNode {
    string data;
    ListNode* tail = nullptr;
    ListNode* head = nullptr;
};

struct List {
    int length = 0;
    ListNode* head = nullptr;
    ListNode* tail = nullptr;

    void add2List(string token) {
        ListNode* cur = new ListNode;
        cur->data = token;
        if (!length) {
            head = cur;
            tail = cur;
            length++;
            return;
        }
        length++;
        tail->tail = cur;
        cur->head = tail;
        tail = cur;
    }

    void reverse() {
        ListNode* cur1 = head, * cur2 = tail;
        for (int i = 0; i < length / 2; i++) {
            for (int i = 0; i < length; i++) {
                if (cur1->data == ")") cur1->data = "(";
                else if (cur1->data == "(") cur1->data = ")";
                else if (cur2->data == ")") cur2->data = "(";
                else if (cur2->data == "(") cur2->data = ")";
            }
            swap(cur1->data, cur2->data);
            cur1 = cur1->tail;
            cur2 = cur2->head;
        }
    }
};

void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


List* divide(string input) {
    List* tokens = new List;
    string number;
    input += ' ';
    for (int i = 0; i < input.length(); i++) {
        char cur = input[i];
        if (cur == ' ') {
            if (!number.empty()) {
                tokens->add2List(number);
            }
            number = "";
        }
        else if (isdigit(cur)) {
            number += cur;
        }
        else {
            if (!number.empty()) {
                tokens->add2List(number);
            }
            number = cur;
            tokens->add2List(number);
            number = "";
        }
    }
    return tokens;
}


void printCollecton(List* tokens) {
    ListNode* cur = tokens->head;
    for (int i = 0; i < tokens->length; i++) {
        cout << cur->data << " ";
        cur = cur->tail;
    }
}


struct Node {
    string data;
    Node* head;
};


struct Stack {
    int length = 0;
    Node* tail = nullptr;

    void push(string value) {
        Node* cur = new Node;
        cur->data = value;
        cur->head = tail;
        tail = cur;
        length++;
    }

    string pop() {
        string rtn = tail->data;
        Node* dlt = tail;
        tail = tail->head;
        delete dlt;
        length--;
        return rtn;
    }

    bool isEmpty() {
        return !length;
    }

    string peek() {
        return tail->data;
    }
};


int getPriority(string token) {
    if (token == "(") return 1;
    else if (token == "+" || token == "-") return 2;
    else if (token == "*" || token == "/") return 3;
    else return 0;
}


int getPrefixPriority(string token) {
    if (token == "(") return 1;
    else if (token == "+") return 2;
    else if (token == "-") return 3;
    else if (token == "*") return 4;
    else if (token == "/") return 5;
    else return 0;
}


List* toPostfix(List* infix) {
    Stack* operators = new Stack;
    List* postfix = new List;
    string chr;
    ListNode* cur;
    int priority;

    cur = infix->head;

    for (int i = 0; i < infix->length; i++) {
        chr = cur->data;
        priority = getPriority(chr);

        if (chr == ")") {
            cout << "Current token ), place in operators all operators up to ( : ";
            while (operators->peek() != "(") {
                cout << operators->peek() << " ";
                postfix->add2List(operators->pop());
            }
            cout << "\n";
            operators->pop();

        }
        else if (!priority) {
            cout << chr << " - operand, move to output. line \n";
            postfix->add2List(chr);

        }
        else if (chr == "(") {
            cout << "Current token (, therefore put it in operators \n";
            operators->push(chr);

        }
        else if (!operators->isEmpty() && getPriority((operators->peek())) < priority) {
            cout << "Current token " << chr << " your priority = " << priority << " ,but priority " <<
                operators->peek() << " = " << getPriority(operators->peek()) << " ,move " << chr << " in operators";
            cout << "\n";
            operators->push(chr);

        }
        else {
            while (!operators->isEmpty() && getPriority(operators->peek()) >= priority) {
                cout << "Current token " << chr << " your priority = " << priority <<
                    " ,but priority " << operators->peek() << " = " << getPriority(operators->peek()) <<
                    " ,move " << operators->peek() << " to the output. line";
                cout << "\n";
                postfix->add2List((operators->pop()));
            }
            operators->push(chr);
        }
        cur = cur->tail;
    }

    int length = operators->length;
    if (!operators->isEmpty()) {
        cout << "Extract from operators the remaining operators in the output. line: ";
    }
    for (int i = 0; i < length; i++) {
        cout << operators->peek() << " ";
        postfix->add2List(operators->pop());
    }
    cout << "\n";
    return postfix;
}


List* toPrefix(List* infix) {
    infix->reverse();
    Stack* operators = new Stack;
    List* prefix = new List;
    string chr;
    ListNode* cur;
    int priority;

    cur = infix->head;

    for (int i = 0; i < infix->length; i++) {
        chr = cur->data;
        priority = getPrefixPriority(chr);

        if (chr == ")") {
            while (operators->peek() != "(") {
                prefix->add2List(operators->pop());
            }
            operators->pop();

        }
        else if (!priority) {
            prefix->add2List(chr);

        }
        else if (chr == "(") {
            operators->push(chr);

        }
        else if (!operators->isEmpty() && getPrefixPriority((operators->peek())) < priority) {
            operators->push(chr);

        }
        else {
            while (operators->length && getPrefixPriority(operators->peek()) >= priority) {
                prefix->add2List((operators->pop()));
            }
            operators->push(chr);
        }
        cur = cur->tail;
    }

    int length = operators->length;
    for (int i = 0; i < length; i++) {
        prefix->add2List(operators->pop());
    }

    infix->reverse();
    prefix->reverse();
    return prefix;
}


double action(double value1, double value2, string operation) {
    switch (operation[0]) {
    case '+': return value2 + value1;
    case '-': return value2 - value1;
    case '*': return value2 * value1;
    case '/': return value2 / value1;
    default: break;
    }
}


double calculateInfixExpression(List* infix, bool& mistakes) {
    Stack* operands = new Stack,
        * operators = new Stack;
    ListNode* cur = infix->head;
    string token;
    while (cur) {
        token = cur->data;
        if (token == "(") {
            operators->push(token);
        }
        else if (token == ")") {
            while (operators->peek() != "(") {
                if (operands->length < 2) {
                    mistakes = true;
                    return 0;
                }
                operands->push(to_string(action(stod(operands->pop()),
                    stod(operands->pop()), operators->pop())));
            }
            operators->pop();
        }
        else {
            if (!getPriority(token)) {
                operands->push(token);
            }
            else {
                while (!operators->isEmpty() && getPriority(operators->peek()) >= getPriority(token)) {
                    if (operands->length < 2) {
                        mistakes = true;
                        return 0;
                    }
                    operands->push(to_string(action(stod(operands->pop()),
                        stod(operands->pop()), operators->pop())));
                }
                operators->push(token);
            }

        }
        cur = cur->tail;

    }
    while (!operators->isEmpty()) {
        if (operands->length < 2) {
            mistakes = true;
            return 0;
        }
        operands->push(to_string(action(stod(operands->pop()),
            stod(operands->pop()), operators->pop())));
    }
    return stod(operands->pop());
}


double calculatePostfixExpression(List* postfix, bool& mistakes) {
    double result;
    Stack* operands = new Stack;
    ListNode* cur = postfix->head;
    for (int i = 0; i < postfix->length; i++) {

        if (!getPriority(cur->data)) {
            operands->push(cur->data);
        }
        else {
            if (operands->length < 2) {
                mistakes = true;
                return 0;
            }
            else {
                operands->push(to_string(action(stod(operands->pop()), stod(operands->pop()), cur->data)));
            }
        }
        cur = cur->tail;
    }
    result = stod(operands->pop());
    return result;
}


double calculatePrefixExpression(List* prefix, bool& mistakes) {
    double result;
    Stack* operands = new Stack;
    ListNode* cur = prefix->tail;
    for (int i = 0; i < prefix->length; i++) {

        if (!getPriority(cur->data)) {
            operands->push(cur->data);
        }
        else {
            if (operands->length < 2) {
                mistakes = true;
                return 0;
            }
            else {
                string value2 = operands->pop(), value1 = operands->pop();
                operands->push(to_string(action(stod(value1), stod(value2), cur->data)));
            }
        }
        cur = cur->head;
    }
    result = stod(operands->pop());
    return result;
}


bool isCorrectPostfix(List* postfix) {
    bool mistakes = false;
    calculatePostfixExpression(postfix, mistakes);
    return !mistakes;
}


bool isCorrectPrefix(List* prefix) {
    bool mistakes = false;
    calculatePrefixExpression(prefix, mistakes);
    return !mistakes;
}


bool isCorrectInfix(List* infix) {
    bool mistakes = false;
    calculateInfixExpression(infix, mistakes);
    return !mistakes;
}


void initVariables(List* tokens) {
    for (ListNode* cur = tokens->head; cur != nullptr; cur = cur->tail) {
        if ((!(cur->data[0] >= '(' && cur->data[0] <= '/') && !(cur->data[0] > '0' && cur->data[0] < '10'))) {
            cout << "initialize the variable " << cur->data << " : "; 
            getline(cin, cur->data);
        }
        if (cur->data[0] > 'a' && cur->data[0] < 'z') {
            cout << "initialize the variable " << cur->data << " : "; 
            getline(cin, cur->data);
        }
    }
}


int main() {
    string inputString;
    List* prefix = nullptr, * postfix = nullptr, * infix = nullptr;
    int selectOperation, selectSubOperation;
    bool mistakes;
    double evaluateResult;
    for (int i = 0; i < 100; i++) {
        menu();
        cout << "Select: "; cin >> selectOperation; clearStream();
        switch (selectOperation) {

        case 1:
            cout << "Enter expression : "; getline(cin, inputString);
            infix = divide(inputString);
            prefix = toPrefix(infix);
            cout << "Prefix: "; 
            printCollecton(prefix); 
            cout << "\n\n";

            postfix = toPostfix(infix);
            cout << "Postfix: "; 
            printCollecton(postfix); 
            cout << "\n\n";
            break;

        case 2:
            mistakes = false;
            cout << "1 - Postfix \n"; 
            cout << "2 - Prefix \n"; 
            cout << "3 - Infix \n";
            cout << "Select: "; cin >> selectSubOperation; clearStream();
            switch (selectSubOperation) {
            case 1:
                if (isCorrectPostfix(postfix)) cout << "Everything is fine";
                else cout << "This expression have error";
                break;

            case 2:
                if (isCorrectPrefix(prefix)) cout << "Everything is fine";
                else cout << "This expression have error";
                break;

            case 3:
                if (isCorrectInfix(infix)) cout << "Everything is fine";
                else cout << "This expression have error";
                break;
            }
            cout << "\n\n";

            break;

        case 3:
            mistakes = false;
            cout << "1 - Postfix \n";
            cout << "2 - Prefix \n";
            cout << "3 - Infix \n";
            cout << "Select: "; cin >> selectSubOperation; clearStream();
            switch (selectSubOperation) {

            case 1:
                initVariables(postfix);
                evaluateResult = calculatePostfixExpression(postfix, mistakes);
                if (mistakes) cout << "Error in expression";
                else cout << "Evaluation: " << evaluateResult;
                break;

            case 2:
                initVariables(prefix);
                evaluateResult = calculatePrefixExpression(prefix, mistakes);
                if (mistakes) cout << "Error in expression";
                else cout << "Evaluation: " << evaluateResult;
                break;

            case 3:
                initVariables(infix);
                evaluateResult = calculateInfixExpression(infix, mistakes);
                if (mistakes) cout << "Error in expression";
                else cout << "Evaluation: " << evaluateResult;
                break;

            default: cout << "Something went wrong";
            }
            cout << "\n\n";
            break;
        }
    }

    return 0;
}
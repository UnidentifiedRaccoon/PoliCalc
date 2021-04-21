#include <iostream>
#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>


#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h>
#include <crtdbg.h>  

using namespace std;

// Data for tasting
string testData[] = {
                    "2 - 1 * 8 ^ ( ( 5 - 3 ) / 2 ) * 4",
                    "1 - 4 * ( 8 - 2 / 3 ) ^ ( 7 - 10 ) + 31",
                    "(83 + 62 - 11) * 8 ^ (45 - (19 / 7) * 12) / 36 - 23",
};

string testDataAnswers[] = { 
                            "1 4 8 2 3 / - 7 10 - ^ * - 31 +",
                            "77 86 - 5 * 57 85 12 - 52 * 37 / 11 3 / - ^ +", 
                            "44 62 71 12 * + 8 / -",
                            "65 - 3 * - 7 / - 4 +",
                            "83 62 + 11 - 8 45 19 7 / 12 * - ^ * 36 / 23 -",
};


// Instructons of basic stack working

#define STACK_INIT_SIZE 20 // ƒеректива (все STACK_INIT_SIZE будут заменены на число 20)
#define MULTIPLIER 2
typedef char T; // “еперь вместо инт можно записывать T + можно мен€ть тип данных всей программы в одном месте (double итд)

typedef struct dynamicStack { // —оздание структуры, то же  что и класс
    T * data;  // ”казатель типа T - заготовка под дин. массив дл€ хранени€ данных
    size_t size; // ƒопустимый размер
    size_t top; // “екущий размер
} dynStack; // Typedef позвол€ет задать альтернативное им€


dynStack *createStack() { // * значит что функци€ не просто возврашает значение типа dinStack а возвращает указатель с этим типом
//    // ”читыва€, что мы создаем указатель stack, а потом его возвращаем, все становитс€ логично
    dynStack *stack; // ќпределение указател€
    stack = new dynStack; // »нициализаци€ указател€
    (*stack).size = STACK_INIT_SIZE; // ¬ значении stack ищим свойство size и присваиваем ему значение
    stack->data = new T[stack->size]; // stack->data то же что и (*stack).data
    stack->top=0;
    return stack;
}

bool isStackEmpty(dynStack *stack) {
    if (stack->top == 0) {
        return true;
    }
    return false;
}


void deleteStack(dynStack **stack) {
    delete [] (*stack)->data; // ”дал€ем данные из стека (*(*stack)).data
//    delete (*stack)->data; ---  » так удал€етс€ во врем€ след действи€
    delete (*stack); // ”дал€ем сам стек (*(*stack)) т е чистим его
    *stack = nullptr; // Ќаправл€ем указатель в тупик (*(*stack))
}

void clearStack(dynStack **stack) {
    delete [] (*stack)->data; //  ажетс€ здесь идет утечка пам€ти удали след строчку и проверь (push push clear pop pop)
    (*stack)->top = 0;
}

void increaseStack(dynStack **stack) {
//    // ¬се это можно было сделать подключив библиотеки, проблема в том, что € не знаю какие
    dynStack *newStack;
    newStack = new dynStack;
    newStack->size = (*stack)->size * MULTIPLIER;
    newStack->top = (*stack)->top;
    newStack->data = new T[newStack->size];
    for (size_t i = 0; i < (*stack)->size; i++) {
        newStack->data[i] = (*stack)->data[i];
    }
    delete [] (*stack)->data; //ѕравильно ли € удал€ю стек и поч в ф-ции del надо по другому
    delete (*stack);

    *stack = newStack; 
}

void push (dynStack **stack, T value) {
    if ((*stack)->top >= (*stack)->size) {
        increaseStack(stack); // stack - это адрес самого указател€ (а не данных на которые он указывает)
    }
    (*stack)->data[(*stack)->top] = value;
    (*stack)->top++;
}



T pop(dynStack *stack) {
    T value;
    if (isStackEmpty(stack) == true) {
        value =  '0';//0 = Error
        return value;
    }
    stack->top--;
    value = stack->data[stack->top];
    return value;
}

T peek(dynStack *stack) {
    T value;
    if (isStackEmpty(stack) == true) {
        value =  '0';//0 = Error
        return value;
    }
    value = stack->data[stack->top-1];
    return  value;
}

void show(dynStack *stack) {
    int i = (stack->top - 1);
    for (; i >= 0; i--) {
        cout << stack->data[i] << " ";
    }
}


// Additional instructions of stack working
void fillStackWithExp(dynStack** stack, string exp) {
    for (size_t i = 0; i < exp.size(); i++) {
        push(stack, exp[i]);
    }
}

// Transforming to polish notation instructons
bool isBracket(char symbol) {
    char listOfBrackets[] = { '(', ')', '[', ']', '{', '}' };
    int listLenght = 6;
    for (size_t i = 0; i < listLenght; i++) {
        if (symbol == listOfBrackets[i]) {
            return true;
        }
    }
    return false;
}

bool isOperator(char symbol) {
    char listOfOperators[] = { '+', '-', '*', '/', '^'};
    int listLenght = 5;
    for (size_t i = 0; i < listLenght; i++) {
        if (symbol == listOfOperators[i]) {
            return true;
        }
    }
    return false;
}

int getPriority(char operation) {
    switch (operation)
    {
    case '+':
    case '-':
        return 1;
        break;
    case '*':
    case '/':
        return 2;
        break;
    case '^':
        return 3;
        break;
    default:
        return 0; // It means that our variable operation contains no operatorationSymbol (bracket or out of listOfOperators)
        break;
    }
}

bool isCloseBracket(char symbol) {
    char listOfCloseBrackets[] = { ')', ']', '}' };
    int listLenght = 3;
    for (size_t i = 0; i < listLenght; i++) {
        if (symbol == listOfCloseBrackets[i]) {
            return true;
        }
    }
    return false;
}

bool isOpenBracket(char symbol) {
    char listOfOpenBrackets[] = { '(', '[', '{' };
    int listLenght = 3;
    for (size_t i = 0; i < listLenght; i++) {
        if (symbol == listOfOpenBrackets[i]) {
            return true;
        }
    }
    return false;
}

// We also can try to put char instead of dynStack, but there are errors which I can't solve
bool isErrorInPriority (dynStack* stack, char symbol) {
    if (getPriority(symbol) == 0) {
        return false;
    }
    if (getPriority(symbol) <= getPriority(peek(stack))) {
        return true;
    }
    return false;
}

string turnExpToPolichNotation(string exp) {
    dynStack* postExp;
    postExp = createStack();
    dynStack* stackOfOperations;
    stackOfOperations = createStack();
    for (size_t i = 0; i < exp.size(); i++) {

        if (isBracket(exp[i]) or isOperator(exp[i])) {

            if (isCloseBracket(exp[i]) or isErrorInPriority(stackOfOperations, exp[i])) {

                if (isCloseBracket(exp[i])) {
                    while (!isOpenBracket(peek(stackOfOperations))) {
                        //cout << "1 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;
                        push(&postExp, pop(stackOfOperations));
                        //cout << "1 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;

                    }
                    //cout << "2 " << "operStack" << peek(stackOfOperations) << endl;

                    pop(stackOfOperations);
                    //cout << "2 " << "operStack" << peek(stackOfOperations) << endl;

                    continue;
                }

                //while (getPriority(exp[i] <= getPriority(peek(stackOfOperations)))
                while (isErrorInPriority(stackOfOperations, exp[i])) {
                    //cout << "3 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;
                    push(&postExp, pop(stackOfOperations));
                    //cout << "3 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;


                }
                push(&stackOfOperations, exp[i]);
                //cout << "4 " << "operStack" << peek(stackOfOperations) << " exp" << exp[i] << endl;

                continue;
            }

            push(&stackOfOperations, exp[i]);
            //cout << "5 " << "operStack" << peek(stackOfOperations) << " exp" << exp[i] << endl;

            continue;
        }
        //if (exp[i] == ' ') {
        //    push(&postExp, currentNumberValue)

        //}
        push(&postExp, exp[i]);
        //cout << "6 " << "post" << peek(postExp) << " exp" << exp[i] << endl;

        continue;
    }

    while (!isStackEmpty(stackOfOperations)) {
        //cout << "7 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;
        push(&postExp, pop(stackOfOperations));
        //cout << "7 " << "post" << peek(postExp) << " operStack" << peek(stackOfOperations) << endl;
    }

    //show(postExp);

    // postExp to string

    string postExpStr = "";

    int i = 0;
    T currentSymbol = ' ';
    T prevSymbol = ' ';
    while (!isStackEmpty(postExp)) {
        prevSymbol = currentSymbol;
        currentSymbol = pop(postExp);
        if (currentSymbol == ' ' && prevSymbol == ' ') {
            continue;
        }
        postExpStr += prevSymbol;
    }
    postExpStr += currentSymbol;

    return postExpStr;
}

// instructions of working with strings
string getReversedString(string str) {
    string revStr = "";
    for (int i = 0; i <= str.size(); i++) {
        revStr += str[str.size() - i];
    }
    return revStr;
}


float calcPolishExp(string polishExp) {
    polishExp = polishExp.substr(1, polishExp.length());
    float result = 0.0;
    vector<string> numbersArr;
    numbersArr.resize(25);
    int k = 0;

    for (size_t i = 0; i < polishExp.size(); i++) {
        if (isOperator(polishExp[i])) {
            // ѕолучение строки содержащей 2 оператор
            while (numbersArr[k].empty()) {
                k--;
            }
            string secondOperand = numbersArr[k];
            numbersArr[k] = "";


            // ѕолучение строки содержащей 1 оператор
            while (numbersArr[k].empty()) {
                k--;
            }
            string firstOperand = numbersArr[k];
            numbersArr[k] = "";

 

            cout << " " << firstOperand << endl;
            cout << " " << secondOperand << endl;

            float F_firstOperand = stof(firstOperand, 0);
            float F_secondOperand = stof(secondOperand, 0);


            switch (polishExp[i])
            {
            case '+':
                result = F_firstOperand + F_secondOperand;
                break;

            case '-':
                result = F_firstOperand - F_secondOperand;
                break;

            case '*':
                result = F_firstOperand * F_secondOperand;
                break;

            case '/':
                result = F_firstOperand / F_secondOperand;
                break;

            case '^':
                result = pow(F_firstOperand, F_secondOperand);
                break;
            }
            numbersArr[k] = to_string(result);
            continue;
        }
        if (polishExp[i] == ' ') {
            if (!numbersArr[k].empty()) {
                k++;
            }
            continue;
        }
        numbersArr[k] += polishExp[i];
        
    }

    return stof(numbersArr[0], 0);
}


int main() {
    dynStack *stack;
    stack = createStack();
    string exp = "";
    cout << "------------------------------Read this recomendations, before using progpamm -------------------------" << endl <<
        "***Pay attention, this programm can't work with variables***" << endl <<
        "***It is also designed only for working with simple math operations like '+', '-', '*', '/', '^' ***" << endl <<
        "***Use right math operations, bracets and numbers order and you will have no errors" << endl <<
        "***This programm dosen't allow calculate expressions with unary minus"<< endl << endl;


    cout << "Enter your  expession: ";
    getline(cin, exp);
    //cout << exp;
    fillStackWithExp(&stack, exp);
    cout << endl;
    //cout << "Stack info: ";
    //show(stack);
    //cout << endl;
    string polishExp;
    polishExp = turnExpToPolichNotation(exp);
    cout << endl;
    cout << "polishExp info: " << polishExp << endl;
    string revercedPolishExp = getReversedString(polishExp);
    cout << "reverced polishExp info: " << revercedPolishExp << endl;
    float expResult = calcPolishExp(revercedPolishExp);
    cout << "result of calculations: " << expResult << endl;

    return 0;
}


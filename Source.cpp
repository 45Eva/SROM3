#include <iostream>
#include <array>
#include <limits>
#include <cmath>


const size_t size = 173;

using Element = std::array<bool, size>;

void createZeroElement(Element& coefficients) {
    coefficients.fill(false);
}

// Функція для перевертання масиву як того потребує стандарт 
Element Correct_OutPut(const Element& arr) {
    Element reversedArray;

    for (size_t i = 0; i < size; ++i) {
        reversedArray[i] = arr[size - i - 1];
    }

    return reversedArray;
}

/*void createCustomElement(Element& coefficients, const std::array<size_t, size>& coefArray, size_t coefCount) {
    coefficients.fill(false);

    for (size_t i = 0; i < coefCount; ++i) {
        if (coefArray[i] < size) {
            coefficients[coefArray[i]] = true;
        }
    }
} */

void createCustomElement(Element& coefficients, const std::string& coefString) {
    coefficients.fill(false);

    for (size_t i = 0; i < size && i < coefString.length(); ++i) {
        coefficients[size - i - 1] = (coefString[i] == '1');
    }
}

void printElement(const Element& coefficients) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << coefficients[i];
    }
    std::cout << std::endl;
}

void toCoefficientArray(const Element& coefficients, std::array<size_t, size>& result, size_t& arraySize) {
    arraySize = 0;
    for (size_t i = 0; i < size && arraySize < size; ++i) {
        if (coefficients[i]) {
            result[arraySize++] = i;
        }
    }
}

void printCoef(const std::array<size_t, size>& result, size_t arraySize) {
    for (size_t i = 0; i < arraySize; ++i) {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;
}

// Функція для перевірки, чи всі елементи масиву дорівнлюють нулю. (Нейтральние ел. по + )
bool Const_0(Element& field_element) {
    for (bool element : field_element) {
        if (element) {
            return false; // Знайдено елемент, не дорівнює нулю
        }
    }
    return true; // Всі елементи дорівнюють нулю
}

// Функція для перевірки, чи в першому елементі масиву стоїть одиниця. (Нейтральний елемент по * )
bool Const_1(Element& field_element) {
    if (field_element.size() > 0 && field_element[0]) {
        for (size_t i = 1; i < field_element.size(); ++i) {
            if (field_element[i]) {
                return false; // Знайдено елемент, не дорівнює нулю
            }
        }
        return true; // Перший елемент дорівнює одиниці, а всі інші - нули
    }
    return false; // Перший елемент не дорівнює одиниці або масив порожній
}

// Функція для додавання двох елементів поля
Element Add(Element& field1, Element& field2) {
    Element result;
    for (size_t i = 0; i < size; ++i) {
        result[i] = field1[i] ^ field2[i]; // XOR для додавання бітів
    }
    return result;
}

// Множення елементів поля 
Element Multiply(const Element& field1, const Element& field2) {
    Element result;
    result.fill(false); // Initialize the result to zero

    std::array<bool, (size * 2) - 1> temp;
    temp.fill(false);

    for (std::size_t i = 0; i < size; ++i) {
        for (std::size_t j = 0; j < size; ++j) {
            temp[i + j] ^= field1[i] & field2[j];
        }
    }

    // Define the modulus polynomial
    std::array<bool, (size * 2) - 1> mod_p;
    mod_p.fill(false);
    mod_p[0] = true;
    mod_p[1] = true;
    mod_p[2] = true;
    mod_p[10] = true;
    mod_p[173] = true;

    for (std::size_t i = (size * 2) - 2; i >= size; --i) {
        if (temp[i]) {
            for (std::size_t j = 0; j < size; ++j) {
                temp[i - size + j] ^= mod_p[j];
            }
        }
    }

    // Copy the result to the output array
    for (std::size_t i = 0; i < size; ++i) {
        result[i] = temp[i];
    }

    return result;
}

//Піднесення до квадрату ел. поля 
Element Squere(const Element& field1) {
    Element result;
    result.fill(false);

    result = Multiply(field1, field1);

    return result;
}


//Обчислення сліду
bool Trace(const Element& field) {
    Element temp = field;
    bool trace = false;

    for (std::size_t i = 0; i < size; ++i) {

        trace ^= temp[i];
        temp = Squere(temp);
    }

    return trace;
}

//Обернений елемент за множенням :
Element Inverse(const Element& field) {
    Element result = field;;
    Element temp = field;

    for (size_t i = 0; i < size - 2; ++i) {
        temp = Squere(temp);
        result = Multiply(result, temp);
    }

    result = Squere(result);

    return result;
}

// Піднесення до степеню  ??????????
Element powerByGorner(const Element& A, const Element& C) {
    // Ініціалізуємо новий масив У з одиницями
    Element U;
    U.fill(true);

    // Проходимо по всім коміркам масиву C від 172 до 0
    for (int i = size - 1; i >= 0; --i) {

        // Підносимо U до квадрату
        U = Squere(U);
        // Якщо поточна комірка масиву C дорівнює одиниці,
        // то множимо U на масив A
        if (C[i]) {
            U = Multiply(U, A);
        }
    }

    return U;
}



int main() {

    // Якщо вводити "ручками" код  
   /* Element A;
    Element B;
    Element C;
    std::array<size_t, size> coefArrayA;
    std::array<size_t, size> coefArrayB;
    std::array<size_t, size> coefArrayC;
    size_t indexA = 0;
    size_t indexB = 0;
    size_t indexC = 0;
    int coef;

    std::cout << "Enter coefficients for element A (empty line to stop '/n'):" << std::endl;
    while (indexA < size && std::cin >> coef) {
        coefArrayA[indexA++] = coef;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter coefficients for element B (empty line to stop '/n'):" << std::endl;
    while (indexB < size && std::cin >> coef) {
        coefArrayB[indexB++] = coef;
    }
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "Enter coefficients for element C (empty line to stop '/n'):" << std::endl;
    while (indexB < size && std::cin >> coef) {
        coefArrayC[indexC++] = coef;
    }

    // створення ел. поля
    createCustomElement(A, coefArrayA, indexA);
    createCustomElement(B, coefArrayB, indexB);
    createCustomElement(C, coefArrayC, indexC);

    std::cout << "Element A:" << std::endl;
    printElement(A);
    std::cout << "Element B:" << std::endl;
    printElement(B);
    std::cout << "Element C:" << std::endl;
    printElement(C);

    // перетворення ел. поля у коефіцієнти
    std::array<size_t, size> coefArrayResultA;
    size_t arraySizeA;
    toCoefficientArray(A, coefArrayResultA, arraySizeA);

    std::array<size_t, size> coefArrayResultB;
    size_t arraySizeB;
    toCoefficientArray(B, coefArrayResultB, arraySizeB);

    std::array<size_t, size> coefArrayResultC;
    size_t arraySizeC;
    toCoefficientArray(C, coefArrayResultC, arraySizeC);

    std::cout << "Coefficient array for A:" << std::endl;
    printCoef(coefArrayResultA, arraySizeA);
    std::cout << "Coefficient array for B:" << std::endl;
    printCoef(coefArrayResultB, arraySizeB);
    std::cout << "Coefficient array for C:" << std::endl;
    printCoef(coefArrayResultC, arraySizeC); */

    // Захаркодити елементи 

    Element A;
    Element B;
    Element C;
    std::string coefStringA = "00010000011000000010010101111001111101110000010001111001101100110001011000111100011100101001100000111011010101001011010010011001111111100010110011010101001100100011111101100";
    std::string coefStringB = "00101001100000110011100011000101000110100110101010110011000001010111011100011001001100011000010000011111100110001110001100110100101110000110101011001101111101010001101010110";
    std::string coefStringC = "00011100111101001100101001101110001111111111100100110101001111010001101111011001000001100001010000110011110001011100011001000001000100011101101110011110000010001100000010111";

    // створення ел. поля
    createCustomElement(A, coefStringA);
    createCustomElement(B, coefStringB);
    createCustomElement(C, coefStringC);

    //Перевірка функції чи є ел. нулем
    std::cout << "Const 0 ? :" << std::endl;
    std::cout << (Const_0(A) ? "True" : "False") << std::endl;

    //Перевірка функції чи є ел. одиницею
    std::cout << "Const 1 ? :" << std::endl;
    std::cout << (Const_1(B) ? "True" : "False") << std::endl;

    //Додавання двох елементів поля 
    Element add;
    add = Add(A, B);
    std::cout << "A + B  :" << std::endl;
    printElement(Correct_OutPut(add));

    //Множення елементів поля 
    Element mul;
    mul = Multiply(A, B);
    std::cout << "A * B  :" << std::endl;
    printElement(Correct_OutPut(mul));

    //Піднесення до квадрату ел. поля 
    Element squere;
    squere = Squere(A);
    std::cout << "A ^ 2  :" << std::endl;
    printElement(Correct_OutPut(squere));


    //Піднесення до степеня  ????????????????
    Element power;
    power = powerByGorner(A, C);
    std::cout << "A ^ C  :" << std::endl;
    printElement(Correct_OutPut(power));




    //Обчислення сліду
    bool trace_A = Trace(A);
    std::cout << "The trace of the element A is: " << (trace_A ? "true" : "false") << std::endl;

    bool trace_B = Trace(B);
    std::cout << "The trace of the element B is: " << (trace_B ? "true" : "false") << std::endl;

    //Обернений елмент за множенням 
    Element inverse;
    inverse = Inverse(A);
    std::cout << "A ^ (-1)  :" << std::endl;
    printElement(Correct_OutPut(inverse));

    //Тестування 

    //(a + b) * c = b*c + c*a
    std::cout << "Testting : (a + b) * c = b*c + c*a " << std::endl;
    Element test1;
    test1 = Multiply(Add(A, B), C);
    std::cout << " (a + b) * c :" << std::endl;
    printElement(test1);
    std::array<size_t, size> coefArrayResult_test1;
    size_t arraySize_test1;
    toCoefficientArray(test1, coefArrayResult_test1, arraySize_test1);

    Element test2;
    Element a = Multiply(B, C);
    Element b = Multiply(C, A);
    test2 = Add(a, b);
    std::cout << " b*c + c*a :" << std::endl;
    printElement(test2);
    std::array<size_t, size> coefArrayResult_test2;
    size_t arraySize_test2;
    toCoefficientArray(test2, coefArrayResult_test2, arraySize_test2);

    std::cout << " (a + b) * c :" << std::endl;
    printCoef(coefArrayResult_test1, arraySize_test1);
    std::cout << " b*c + c*a :" << std::endl;
    printCoef(coefArrayResult_test2, arraySize_test2);

}









//
// Created by Alienson on 10.2.2024..
//

#include "iostream"

#include "TestClass.h"

int main(){

    int shared = 3;
    int x = 5;
    int ref = 4;
    std::vector<int> vec = {1,2,3};
    std::shared_ptr<int> sharedPtr = std::make_shared<int>(shared);
    TestClass *clazz = new TestClass();
    TestClass testClass(x, vec,sharedPtr, ref, clazz);

    std::vector<Field> fields = testClass.declaredFields;
    for (Field& f : fields) {
        std::cout << "Field Name: " << f.name << " ,Field type: " << f.type << '\n';
    }

    std::cout<< "Class name is: " << fields[0].className << '\n';

    std::cout << "x value is: " << *((int*)(fields[0].getAddress(&testClass))) << '\n';
    std::cout << "vec values are: ";
    std::vector<int>* tmp = (std::vector<int>*)(fields[1].getAddress(&testClass));
    for (auto y : *tmp){
        std::cout << y << ' ';
    }
    std::cout << '\n';

}

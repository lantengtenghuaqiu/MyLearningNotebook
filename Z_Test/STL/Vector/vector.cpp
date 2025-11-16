#include<iostream>

#include<vector>

int main(){

    std::vector<float> v={1,2,3,4};

    std::cout<<v.data()[0]<<std::endl;


    return 0;
}
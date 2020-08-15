//
// Created by orithaz on 11/24/19.
//

#ifndef ASSIGNMENT1_TAG_H
#define ASSIGNMENT1_TAG_H

#include <string>

class Tag {
public:
    Tag(): tagName(""),counter(0){}
    explicit Tag(std::string, int counter=1);
   /* Tag(const Tag & tag);*/

    bool operator<( Tag & other) ;
    std::string getName(){return tagName;}
    int getCount() {return counter;}
    void adder(){ counter++;}

private:
    std::string tagName;
    int counter;
};


#endif //ASSIGNMENT1_TAG_H

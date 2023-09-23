#ifndef FOLDER_H
#define FOLDER_H


class Message;

class Folder{
    
public:
    void addMsg(const Message*);
    void remMsg(const Message*);
};

#endif
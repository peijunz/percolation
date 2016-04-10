#ifndef LINKLIST_H
#define LINKLIST_H

template <typename C>
class snode{
public:
    snode *next;
    C data;
    snode():next(0){}
    snode(C d):next(0), data(d){}
};

template <typename dtype>
/**
 * @brief The linklist class to store the near point of
 */
class llist{
public:
    snode<dtype> *head;
    llist():head(0){}
    ~llist();
    void clear();
    void addsnode(dtype);
};

template <typename dtype>
void llist<dtype>::clear(){
    snode<dtype>* tmp=0;
    while((tmp=head)!=0){
        head=head->next;
        delete tmp;
    }
}
template <typename dtype>
llist<dtype>::~llist(){
    clear();
}

template <typename dtype>
void llist<dtype>::addsnode(dtype t){
    snode<dtype>* tmp=new snode<dtype>(t);
    tmp->next=head;
    head=tmp;
}

#endif //LINKLIST_H

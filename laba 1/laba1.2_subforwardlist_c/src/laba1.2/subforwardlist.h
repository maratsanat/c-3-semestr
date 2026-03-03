#ifndef SUBFORWARDLIST_H
#define SUBFORWARDLIST_H

struct subforwardlist {
    int data;
    subforwardlist* next;
};

bool init(subforwardlist** sfl);
bool push_back(subforwardlist** sfl, int d);
int pop_back(subforwardlist** sfl);
bool push_forward(subforwardlist** sfl, int d);
int pop_forward(subforwardlist** sfl);
bool push_where(subforwardlist** sfl, unsigned int where, int d);
int erase_where(subforwardlist** sfl, unsigned int where);
unsigned int size(subforwardlist** sfl);
void clear(subforwardlist** sfl);

#endif
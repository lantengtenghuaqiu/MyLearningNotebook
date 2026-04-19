#include <iostream>

struct ListNode
{
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

int main()
{
    std::cout << "Hello World" << std::endl;
    //-------------------------------------
    // int Anum[] = {2, 4, 3, 4};
    // int Bnum[] = {5, 6, 4};

    ListNode *Anode = new ListNode();
    ListNode *Anode2 = new ListNode();
    ListNode *Anode3 = new ListNode();
    ListNode *Anode4 = new ListNode();
    ListNode *Anode5 = new ListNode();
    ListNode *Anode6 = new ListNode();

    Anode->val = 9;
    Anode->next = Anode2;

    Anode2->val = 9;
    Anode2->next = Anode3;

    Anode3->val = 9;
    Anode3->next = Anode4;

    Anode4->val = 9;
    Anode4->next = Anode5;

    Anode5->val = 9;
    Anode5->next = Anode6;

    Anode6->val = 9;
    //-------------------------------------
    ListNode *Bnode = new ListNode();
    ListNode *Bnode2 = new ListNode();
    ListNode *Bnode3 = new ListNode();
    ListNode *Bnode4 = new ListNode();
    ListNode *Bnode5 = new ListNode();

    Bnode->val = 9;
    Bnode->next = Bnode2;

    Bnode2->val = 9;
    Bnode2->next = Bnode3;

    Bnode3->val = 9;
    Bnode3->next = Bnode4;

    Bnode4->val = 9;

    //-------------------------------------
    ListNode *node = new ListNode((Anode->val + Bnode->val) % 10);
    int adv = (Anode->val + Bnode->val) / 10;

    Anode = Anode->next;
    Bnode = Bnode->next;
    ListNode *current = node;
    while (Anode != nullptr || Bnode != nullptr || adv != 0)
    {

        int Anum = Anode == nullptr ? 0 : Anode->val;
        int Bnum = Bnode == nullptr ? 0 : Bnode->val;
        int Tnum = Anum + Bnum + adv;
        current->next = new ListNode((Tnum) % 10);

        adv = Tnum / 10;

        current = current->next;

        if (Anode != nullptr)
            Anode = Anode->next;

        if (Bnode != nullptr)
            Bnode = Bnode->next;
    }

    while (node != nullptr)
    {
        std::cout << "Val : " << node->val << std::endl;
        node = node->next;
    }

    return 0;
}
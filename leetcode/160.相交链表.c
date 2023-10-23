/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    struct ListNode *p = headA, *q = headB;

    while (q)
    {
        while (p)
        {
            if (p -> val == q -> val)
            {
                if (p == q)
                {
                    return p;
                }
            }
            p = p -> next;
        }
        p = headA;
        q = q -> next;
    }

    return NULL;
}
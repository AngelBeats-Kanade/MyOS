#ifndef LIST_H
#define LIST_H

#include "portmacro.h"

struct xLIST_ITEM {
  TickType_t xItemValue;
  struct xLIST_ITEM *pxNext;
  struct xLIST_ITEM *pxPrevious;
  void *pvOwner;
  void *pvContainer;
};
typedef struct xLIST_ITEM ListItem_t;

struct xMINI_LIST_ITEM {
  TickType_t xItemValue;
  ListItem_t *pxNext;
  ListItem_t *pxPrevious;
};
typedef struct xMINI_LIST_ITEM MiniListItem_t;

struct xLIST {
  UBaseType_t uxNumberOfItems;
  ListItem_t *pxIndex;
  MiniListItem_t xListEnd;
};
typedef struct xLIST List_t;

#define listSET_LIST_ITEM_OWNER(pxListItem, pxOwner) ((pxListItem)->pvOwner = (void *)(pxOwner)) //初始化节点的拥有者
#define listGET_LIST_ITEM_OWNER(pxListItem) ((pxListItem)->pvOwner)                              //获取节点拥有者
#define listSET_LIST_ITEM_VALUE(pxListItem, xValue) ((pxListItem)->xItemValue = (xValue))        //初始化节点排序辅助值
#define listGET_LIST_ITEM_VALUE(pxListItem) ((pxListItem)->xItemValue)                           //获取节点排序辅助值
#define listGET_ITEM_VALUE_OF_HEAD_ENTRY(pxList) (((pxList)->xListEnd).pxNext->xItemValue)       //获取链表根节点的节点计数器的值
#define listGET_HEAD_ENTRY(pxList) (((pxList)->xListEnd).pxNext)                                 //获取链表的入口节点
#define listGET_NEXT(pxListItem) ((pxListItem)->pxNext)                                          //获取节点的下一个节点
#define listGET_END_MARKER(pxList) ((ListItem_t const*)(&((pxList)->xListEnd)))                  //获取链表的最后一个节点
#define listLIST_IS_EMPTY(pxList) ((BaseType_t)((pxList)->uxNumberOfItems == (UBaseType_t) 0))   //判断链表是否为空
#define listCURRENT_LIST_LENGTH(pxList) ((pxList)->uxNumberOfItems)                              //获取链表的节点数
#define listGET_OWNER_OF_HEAD_ENTRY(pxList) ((&((pxList)->xListEnd))->pxNext->pvOwner)
#define listGET_OWNER_OF_NEXT_ENTRY(pxTCB, pxList)                       \
{                                                                        \
  List_t *const pxConstList = (pxList);                                  \
  (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;               \
  if((void*)(pxConstList)->pxIndex == (void*)&((pxConstList)->xListEnd)) \
  {                                                                      \
    (pxConstList)->pxIndex = (pxConstList)->pxIndex->pxNext;             \
  }                                                                      \
  (pxTCB) = (pxConstList)->pxIndex->pvOwner;                             \
}

void vListInitialiseItem(ListItem_t *const pxItem);
void vListInitialise(List_t *const pxList);
void vListInsertEnd(List_t *const pxList, ListItem_t *const pxNewListItem);
void vListInsert(List_t *const pxList, ListItem_t *const pxNewListItem);
UBaseType_t uxListRemove(ListItem_t *const pxItemToRemove);

#endif //LIST_H
#include "list.h"

listItem *createItem(void *key, void *data) {
	listItem *result = (listItem *)malloc(sizeof(listItem));

	result->key = key;
	result->data = data;
	result->next = NULL;
	result->prev = NULL;

	return result;
}

list createList() {
	list *result = (list *)malloc(sizeof(list));

	result->head = createItem(-1, '$');
	result->tail = createItem(-1, '$');

	result->head->next = result->tail;
	result->tail->prev = result->head;

	return result;
}

void addFirst(list *lst, void *key, void *data) {
	listItem *newItem = createItem(key, data);

	newItem->next = lst->head->next;
	newItem->prev = lst->head;
	lst->head->next->prev = newItem;
	lst->head->next = newItem;
}

void addLast(list *lst, void *key, void *data) {
	listItem *newItem = createItem(key, data);

	newItem->next = lst->tail;
	newItem->prev = lst->tail->prev;
	lst->tail->prev->next = newItem;
	lst->tail->prev = newItem;
}

void removeItem(listItem *item) {
	if (item != NULL) {
		item->prev->next = item->next;
		item->next->prev = item->prev;

		free(item);
	}
}

listItem *find(list *lst, void *key) {
	listItem *temp = lst->head;

	for (; temp->next != lst->tail; temp = temp->next) {
		if (temp->key == key) return temp;
	}

	return NULL;
}

void delete(listItem *head) {
	if (head != NULL) {
		deleteList(head->next);
		free(head);
	}
}

void deleteList(list *lst) {
	delete(lst->head);
}

int isEmpty(list *lst) {
	return (lst->head->next == lst->tail);
}

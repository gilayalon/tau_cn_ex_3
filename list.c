#include "list.h"

listItem *createNew(void *data) {
	listItem *result;

	result = (listItem *)malloc(sizeof(listItem));
	result->data = data;
	result->next = NULL;
	result->prev = NULL;

	return result;
}

list createList() {
	list result;

	result.head = createNew('$');
	result.tail = createNew('$');

	result.head->next = result.tail;
	result.tail->prev = result.head;

	return result;
}

void addItem(list *lst, void *data) {
	listItem *newItem = createNew(data);

	newItem->next = lst->head->next;
	newItem->prev = lst->head;
	lst->head->next->prev = newItem;
	lst->head->next = newItem;
}

void removeItem(listItem *item) {
	if (item != NULL) {
		item->prev->next = item->next;
		item->next->prev = item->prev;

		free(item);
	}
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

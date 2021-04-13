#include <stdio.h>
#include <stdlib.h>

struct node{
  int key , degree ;
  struct node *sibling ,*child ,*p ;
};

struct heap{
  struct node *head ;
};

struct node * create_node ( int key){
  struct node *p = (struct node *)malloc(sizeof(struct node));
  p->key = key ;
  p->degree = 0 ;
  p->p  = NULL ;
  p->sibling = NULL ;
  p->child = NULL ;

  return p ;
}

struct heap * make_heap(){

  struct heap *h  = (struct heap*)malloc(sizeof(struct heap)) ;
  h->head = NULL ;
  return h ;
}

struct node * Binomial_link (struct node*x , struct node * y){

    x->sibling = y->child;
    y->child = x;
    x->p = y;
    y->degree += 1;

    return y;
}

struct node * reverse (struct node * x){
  struct node *current = x , *next = NULL ,*prev = NULL ;
  while (current != NULL) {
    current->p = NULL ;
    next = current->sibling ;
    current->sibling = prev  ;
    prev =current ;
    current = next ;
  }
  x = prev ;
  return x ;
}

struct node * merge(struct node *a , struct node *b ){

  if (a == NULL )
      return b ;
  else if (b == NULL )
      return a ;
  else if (a->degree < b->degree)
  {
    a->sibling = merge (a->sibling , b);
    return a ;
  }
  else {
    b->sibling = merge (b->sibling , a );
    return b ;
  }
}

int degree (struct node *a){
  if (a == NULL )
     return 10000 ;

  return a->degree ;
}

struct node * Union (struct node *a , struct node *b){

  struct heap * h = make_heap();
  h->head = merge(a,b);
  if (h->head == NULL)
     return h->head ;
  struct node * prev = NULL , *x = h->head , *next = x->sibling ;

  while (next != NULL ) {

if((x->degree!=next->degree)||(next->sibling&&(x->degree==next->sibling->degree)))
  {
      prev = x ;
      x = next ;
   }

   else if( x->key <= next->key ){
     x->sibling = next->sibling ;
     Binomial_link(next, x);
   }
   else{

     if (prev == NULL )
        h->head = next ;
     else
        prev->sibling = next ;

     Binomial_link(x,next);
     x = next ;
   }

  next = x->sibling ;
  }

  return h->head;

}

struct node * insert(struct node *H , struct node *x){
  struct heap *h = make_heap();
  h->head  = x ;
  H = Union(H,h->head);
  return H ;
}

struct node * extract_min (struct node * H){
   struct node * m = create_node(9999999) , *curr = H , *prev = NULL , *pre = NULL;

   while (curr != NULL)
   {
     if (m->key > curr->key){
       m = curr ;
       pre = prev ;
     }
     prev = curr ;
     curr = curr->sibling ;
   }

   struct node * x = m->child ;
   if (pre != NULL)
         pre->sibling = m->sibling ;
   else
         H = m->sibling ;

  struct heap *h = make_heap();
  h->head = reverse (x);
  H = Union(H,h->head);
  return H ;
}

struct node * decrease_key(struct node *h , struct node *x , int k){

  int t ;
  x->key -= k ;
  struct node *y = x ;
  struct node *z = y->p ;

  while (z != NULL && (y->key < z->key))
  {
       t = y->key ;
       y->key = z->key ;
       z->key = t;

       y = z ;
       z = y->p ;
  }
  return h ;
}

struct node * delete (struct node * H , struct node *x){

   decrease_key(H,x,9999999);
   H = extract_min(H);
   return H ;
}

struct node * get_min(struct heap*h){

  struct node *x = h->head ;
  struct node *y = x ;
  int min = x->key ;
  x = x->sibling;

  while (x!=NULL) {
    if (x->key < min)
    {
      min = x->key ;
      y = x ;
    }
    x = x->sibling ;
  }
  return y ;
}

// queue things from here

struct queue {

   int tail;
   struct node *a[100] ;
   int size;
   int head;
};

int isempty(struct queue*s){
   return (s->head == -1 || s->head == s->tail) ;
}

struct node *  Dequeue(struct queue *s){
   if (isempty(s) )
   {
      return NULL ;
   }

   else
   {
       s->head = (s->head+1)%s->size;
       return s->a[s->head-1];
   }

}

void Enqueue(struct queue *s,struct node * k){
   s->a[s->tail] = k ;
   s->tail ++ ;
}

struct node * Search(struct heap *h ,int key){

  struct queue *q = (struct queue*)malloc(sizeof(struct queue));
  q->head = 0 ;
  q->tail = 0 ;
  q->size = 100;

  struct node * current = h->head ;

  while (current != NULL ){
    q->head = q->tail = 0 ;
    Enqueue(q,current);

    while (!isempty(q)){
      struct node *p = q->a[q->head];
      Dequeue(q);

      if (p->key == key)
          return p ;

      if (p->child != NULL ){
        struct node * temp = p->child ;
        while (temp!= NULL )
        {
          Enqueue(q,temp);
          temp = temp->sibling ;
        }
      }
    }

    current = current->sibling ;
  }

  return NULL ;
}

struct node * print_tree (struct heap *h ){

  struct queue *q = (struct queue*)malloc(sizeof(struct queue));
  q->head = 0 ;
  q->tail = 0 ;
  q->size = 100;

  struct node * current = h->head ;

  while (current != NULL ){

    q->head = q->tail = 0 ;
    Enqueue(q,current);

    while (!isempty(q)){
      struct node *p = q->a[q->head];
      Dequeue(q);

      printf("%d ",p->key);

      if (p->child != NULL ){
        struct node * temp = p->child ;
        while (temp!= NULL )
        {
          Enqueue(q,temp);
          temp = temp->sibling ;
        }
      }
    }

    current = current->sibling ;
  }
}

int main(){

  char c , end =  0;
  int k , val ;

  struct heap *h = (struct heap*)malloc(sizeof(struct heap));
  h->head = (struct node *)malloc(sizeof(struct node));
  h->head = NULL ;

  struct node *temp = NULL  ;

  while(1)
  {
     scanf("%c",&c);
     switch(c)
     {
       case 'i':

           scanf("%d",&k);
           h->head = insert(h->head , create_node(k));
       break ;

       case 'd':

               scanf("%d",&k);
               temp = Search(h,k);
               if (temp== NULL)
                  printf("-1\n");
               else
                 h->head = delete(h->head,temp);
       break ;

       case 'p':

           print_tree(h);
           printf("\n");
       break ;

       case 'm':

           temp = get_min(h);
           if (temp)
              printf("%d\n",temp->key);
       break ;

       case 'x':
           temp = get_min(h);
           if (temp)
              printf("%d\n",temp->key);
           h->head = extract_min(h->head);
       break;

       case 'r':

           scanf("%d %d",&k,&val);
           temp  = Search(h,k);
           if (temp && temp->key > val)
           {
               printf("%d\n",temp->key-val);
               decrease_key(h->head,temp,val);
           }
           else
           {
             printf("-1\n");
           }
       break ;

       case 'e':

        end =1 ;
     }
     if (end == 1)
        break ;
  }

  return 0 ;
}

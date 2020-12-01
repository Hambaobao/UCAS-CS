struct tnode *addtree(struct tnode *ptree,char *word){
	struct tnode *p,*q;
	int cmpresult;
	if(ptree==NULL){
		p= talloc();
		p->word=mystrdup(word);
		p->count=1;
		p->left=p->right=NULL;
		return p;
	}else{
		q=ptree;
		while((((cmpresult=strcmp(word,q->word))<0)&&(q->left!=NULL))||((cmpresult>0)&&(q->right!=NULL))){
			if((cmpresult<0)&&(q->left!=NULL)){
				q=q->left;
			}else{
				q=q->right;
			}
		}
		if(cmpresult==0){
			q->count++;
			return q;
		}else if(cmpresult<0){
			p= talloc();
			p->word=mystrdup(word);
			p->count=1;
			p->left=p->right=NULL;
			q->left=p;
			return p;			
		}else{
			p= talloc();
			p->word=mystrdup(word);
			p->count=1;
			p->left=p->right=NULL;
			q->right=p;
			return p;				
		}
	}
}


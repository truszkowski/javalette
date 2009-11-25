/**
 * @brief Wyliczanie wartosci wyrazenia do kodu asemblera.
 * @author Piotr Truszkowski
 */

#include "expr_resolv.h"
#include "asm.h"

j_expr_tree_t *j_new_expr_tree(
		j_program_t                 *program,
		FILE                        *output,
		j_context_t                 *ctx,
		j_expr_t                    *expr)
{
	j_expr_tree_t *new = 
		(j_expr_tree_t*)malloc(sizeof(j_expr_tree_t));

	assert(NULL != new);

	new->program = program;
	new->output = output;
	new->ctx = ctx;
	new->size = 0;
	new->root = NULL;
	j_new_expr_node(new, NULL, expr);

	//- Korzen powinien byc ustawiony w funkcji wyzej
	assert(NULL != new->root);

	return new;
}

void j_delete_expr_tree(
		j_expr_tree_t               *tree)
{
	if (NULL != tree->root) 
		j_delete_expr_node(tree->root);

	free(tree);
}

j_expr_node_t *j_new_expr_node(
		j_expr_tree_t               *tree,
		j_expr_node_t               *parent,
		j_expr_t                    *expr)
{
	assert(NULL != expr);

	size_t count = 0;
	
	if (J_EXPR_VALUE == expr->etype || J_EXPR_ID == expr->etype) count = 0;
	else if (j_expr_is_unary(expr)) count = 1;
	else if (j_expr_is_binary(expr)) count = 2;
	else {
		assert(J_EXPR_FUNCTION == expr->etype);
		
		j_function_args_t *args = expr->function->args;
		while (NULL != args) args = args->args, ++count;
	}

	j_expr_node_t *new = 
		(j_expr_node_t*)malloc(sizeof(j_expr_node_t) + count*sizeof(j_expr_node_t*));
	
	assert(NULL != new);

	new->parent = parent;
	new->expr = expr;
	new->count = count;
	new->status = J_EXPR_NODE_WAIT; 
	new->number = tree->size++;

	if (NULL != tree->root) {
		//- Wstawiamy tak, aby na poczatku listy byly wezly lewe, a na koncu prawe.
		//- Kluczowym jest podlaczanie sie do listy na koniec listy wzgledem korzenia
		//- drzewa (czyli przed... bo jest to lista cykliczna dwukierunkowa).
		//- ... -> Y <-(-> Z <-)-> root <--> A <--> B <- ...
		new->left = tree->root->left;
		new->right = tree->root;
		tree->root->left->right = new;
		tree->root->left = new;
	} else {
		tree->root = new;
		new->left  = new;
		new->right = new;
	}

	switch (expr->etype) {
		case J_EXPR_VALUE: 
		case J_EXPR_ID:
			assert(0 == count);
			new->status = J_EXPR_NODE_READY;
			//- Jesli w idexpr wystepuje przypisanie jakiegos wyrazenia,
			//- mozemy sobie pozwolic na utworzenie oddzielnego drzewa wyrazen.
			//- Bedzie to zgodne z przyjetym sposobem wyliczania wyrazen - od
			//- lewej do prawej.
			return new;
		default:
			if (J_EXPR_FUNCTION == expr->etype) {
				if (0 == count) new->status = J_EXPR_NODE_READY;

				j_function_args_t *args = expr->function->args;
				size_t i = 0;
				
				while (NULL != args)
					new->children[i++] = j_new_expr_node(tree, new, args->expr), args = args->args;
			} else if (j_expr_is_binary(expr)) {
				new->children[0] = j_new_expr_node(tree, new, expr->left);
				new->children[1] = j_new_expr_node(tree, new, expr->right);
			} else if (j_expr_is_unary(expr)) {
				new->children[0] = j_new_expr_node(tree, new, expr->left);
			} else {
				fprintf(stderr, "fatal: unknown expresion type: %d\n", expr->type);
				abort();
			}
	}
			
	return new;
}

void j_delete_expr_node(
		j_expr_node_t               *node)
{
	assert(NULL != node);

	size_t i;
	for (i = 0; i < node->count; ++i)
		if (node->children[i])
			j_delete_expr_node(node->children[i]);

	node->left->right = node->right;
	node->right->left = node->left;

	free(node);
}

void j_expr_tree_generate_asm_code(
		j_expr_tree_t               *tree)
{
	//- Nazwa zmiennej tymczasowej dla korzenia winna byc rowna 0.
	assert(!tree->root->number);

	//- Rezerwujemy miejsce na zmienne tymczasowe, czasem nadmiarowo.
	//- Nie trzeba sie przejmowac, jesli wyrazenie jest typu void, to
	//- przyznane miejsce na stosie nie zostanie uzyte.
	fprintf(tree->output, "\tsub esp , %zd\n", tree->size*4);

	j_expr_node_t *first = tree->root, *node = first;
	assert(NULL != first);

	do {
		//- Po kolejnych obrotach wystarczy sie cofnac tylko o jeden,
		//- ale tak narazie bedzie bezpieczniej...
		node = tree->root; 
	
		do {
			if (J_EXPR_NODE_READY == node->status) {
				//- Szukamy gotowego do policzenia wezla najblizej na lewo
				//- Potem nastepnego najblizej - takie podejscie umozliwi
				//- leniwe wyliczanie dla '&&' i '||'.
				j_expr_node_generate_asm_code(tree, node);
				break;
			}	
			
			//- Moze bedzie cos troche na prawo?
			node = node->right; 
		} while (first != node);
	
		//- Warunek petli: czy wszystko przejrzane i wszystko policzone?
	} while (first != node);


	//- Nawet jesli wyrazenie przyporzadkowane na [esp] jest typu void 
	//- to i tak nikt z niego dalej nie skorzysta.
	fprintf(tree->output, "\tmov eax , [esp]\n");
	fprintf(tree->output, "\tadd esp , %zd\n", tree->size*4);
}

void j_expr_node_generate_asm_code(
		j_expr_tree_t               *tree,
		j_expr_node_t               *node)
{
	assert(J_EXPR_NODE_READY == node->status);
	
	size_t i;

	//- Wezly nizej, winny byc juz przeliczone...
	for (i = 0; i < node->count; ++i) 
		assert(J_EXPR_NODE_DONE == node->children[i]->status);

	//- Odhaczamy wezel jako przerobiony...
	node->status = J_EXPR_NODE_DONE;

	if (NULL != node->parent) {
		//- Odhaczamy rodzica jesli ten ma juz policzone wszystkie dzieci...
		assert(J_EXPR_NODE_WAIT == node->parent->status);
		int all_done = 1;
		
		for (i = 0; i < node->parent->count; ++i) 
			if (J_EXPR_NODE_DONE != node->parent->children[i]->status) { 
				all_done = 0; break;
			}
		
		if (all_done) node->parent->status = J_EXPR_NODE_READY;
	}

	if (J_EXPR_VALUE == node->expr->etype) {
		//- Na tempa wrzucamy wartosc...
		//- Trzeba uwarzac, zeby dla double/floata podac w '[]'...
		fprintf(tree->output, "\tmov ecx , %s%s%zd%s\n",
				J_DOUBLE == node->expr->value->type?"[":"", J_VAL_LABEL, 
				node->expr->value->idnumber, J_DOUBLE == node->expr->value->type?"]":"");
		fprintf(tree->output, "\tmov [esp+%zd] , ecx\n", node->number*4);
	} else if (J_EXPR_ID == node->expr->etype) {
		//- Na eax bedzie co trzeba
		j_expr_node_generate_asm_code_idexpr(tree, node);
		fprintf(tree->output, "\tmov [esp+%zd] , eax\n", node->number*4);
	} else if (J_EXPR_FUNCTION == node->expr->etype) {
		//- Wywolanie funkcji, odkladamy na stos jej parametry... - musimy jeszcze 
		//- wiedziec gdzie jest jej deklaracja - dlatego *_directly.
		//j_funtbl_inf_t *inf = j_context_find_fun_inf(tree->ctx, node->expr->function->id->number);
		j_funtbl_inf_t *inf = j_context_find_fun_inf_directly(tree->ctx, node->expr->function->id->ctx, node->expr->function->id->number);
		assert(NULL != inf);

		size_t i;
		
		if (node->count > 0) {
			fprintf(tree->output, "\tmov ecx , esp\n");
			for (i = 0; i < node->count; ++i) 
				fprintf(tree->output, "\tpush dword [ecx+%zd]\n",
						node->children[node->count-i-1]->number*4);
		}

		fprintf(tree->output, ";; Calling function '%s' ;;\n",
				j_names_get_name(tree->program->names, node->expr->function->id->number));
		fprintf(tree->output, "\tcall near %s%zd\n", J_FUN_LABEL, inf->fun->idnumber);

		if (node->count > 0) 
			fprintf(tree->output, "\tadd esp , %zd\n", node->count*4);
		
		//- Na eax funkcje zapisuja wynik, wiec musimy wartosc 
		//- przepisac na zmienna tymczasowa...
		fprintf(tree->output, "\tmov [esp+%zd] , eax\n", node->number*4);
	} else if (j_expr_is_binary(node->expr)) {
		if (J_DOUBLE != node->children[0]->expr->type) {
			assert(J_DOUBLE != node->children[1]->expr->type);
			fprintf(tree->output, "\tmov eax , [esp+%zd]\n", node->children[0]->number*4);
			fprintf(tree->output, "\tmov ebx , [esp+%zd]\n", node->children[1]->number*4);

			switch (node->expr->etype) {
				case J_EXPR_ADD:
					fprintf(tree->output, "\tadd eax , ebx\n"); break;
				case J_EXPR_SUB:
					fprintf(tree->output, "\tsub eax , ebx\n"); break;
				case J_EXPR_MUL:
					fprintf(tree->output, "\timul eax , ebx\n"); break;
				case J_EXPR_DIV:
					fprintf(tree->output,
							"\txor edx , edx\n"
							"\tcmp eax , 0\n"
							"\tjl %s%zdless\n"
							"\tidiv ebx\n"
							"\tjmp %s%zddone\n"
							"%s%zdless:\n"
							"\tneg eax\n"
							"\tidiv ebx\n"
							"\tneg eax\n"
							"%s%zddone:\n",
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber);
					break;
				case J_EXPR_MOD:
					fprintf(tree->output, 
							"\txor edx , edx\n"
							"\tcmp eax , 0\n"
							"\tjl %s%zdless\n"
							"\tidiv ebx\n"
							"\tjmp %s%zddone\n"
							"%s%zdless:\n"
							"\tneg eax\n"
							"\tidiv ebx\n"
							"\tneg edx\n"
							"%s%zddone:\n"
							"\tmov eax , edx\n",
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber);
					break;
				case J_EXPR_AND:
					fprintf(tree->output, "\tand eax , ebx\n"); break;
				case J_EXPR_OR:
					fprintf(tree->output, "\tor eax , ebx\n"); break;
				case J_EXPR_EQ:
				case J_EXPR_NE:
				case J_EXPR_LT:
				case J_EXPR_GT:
				case J_EXPR_LE:
				case J_EXPR_GE:
					fprintf(tree->output, 
							"\tcmp eax , ebx\n"
							"\t%s short %s%zdcmp\n"
							"\tmov eax , 0\n"
							"\tjmp short %s%zdend\n"
							"%s%zdcmp:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n", 
							(J_EXPR_EQ == node->expr->etype) ? "je" : 
							(J_EXPR_NE == node->expr->etype) ? "jne" : 
							(J_EXPR_LT == node->expr->etype) ? "jl" : 
							(J_EXPR_GT == node->expr->etype) ? "jg" : 
							(J_EXPR_LE == node->expr->etype) ? "jng" : 
							(J_EXPR_GE == node->expr->etype) ? "jnl" : "???",
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber, 
							J_EXPR_LABEL, node->expr->idnumber); 
					break;
				default:
					fprintf(stderr, "fatal: unknown expresion type: %d\n", node->expr->etype);
					abort();
			}

			fprintf(tree->output, "\tmov [esp+%zd] , eax\n", node->number*4);
		} else {
			assert(J_DOUBLE == node->children[1]->expr->type);

			switch (node->expr->etype) {
				case J_EXPR_ADD:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfadd dword [esp+%zd]\n"
							"\tfstp dword [esp+%zd]\n",
							node->children[0]->number*4, node->children[1]->number*4, node->number*4);
					break;
				case J_EXPR_SUB:
					fprintf(tree->output, 
							"\tfld dword [esp+%zd]\n"
							"\tfsub dword [esp+%zd]\n"
							"\tfstp dword [esp+%zd]\n",
							node->children[0]->number*4, node->children[1]->number*4, node->number*4);
					break;
				case J_EXPR_MUL:
					fprintf(tree->output, 
							"\tfld dword [esp+%zd]\n"
							"\tfmul dword [esp+%zd]\n"
							"\tfstp dword [esp+%zd]\n",
							node->children[0]->number*4, node->children[1]->number*4, node->number*4);
					break;
				case J_EXPR_DIV:
					fprintf(tree->output, 
							"\tfld dword [esp+%zd]\n"
							"\tfdiv dword [esp+%zd]\n"
							"\tfstp dword [esp+%zd]\n",
							node->children[0]->number*4, node->children[1]->number*4, node->number*4);
					break;
				case J_EXPR_EQ:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\tand ah , 69\n"
							"\tcmp ah , 64\n"
							"\tje %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[1]->number*4, node->children[0]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				case J_EXPR_NE:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\tand ah , 69\n"
							"\tcmp ah , 64\n"
							"\tjne %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[1]->number*4, node->children[0]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				case J_EXPR_LT:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\ttest ah , 69\n"
							"\tje %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[0]->number*4, node->children[1]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				case J_EXPR_GT:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\ttest ah , 69\n"
							"\tje %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[1]->number*4, node->children[0]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				case J_EXPR_LE:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\ttest ah , 5\n"
							"\tje %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[0]->number*4, node->children[1]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				case J_EXPR_GE:
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfld dword [esp+%zd]\n"
							"\tfcompp\n"
							"\tfstsw ax\n"
							"\ttest ah , 5\n"
							"\tje %s%zdeq\n"
							"\tmov eax , 0\n"
							"\tjmp %s%zdend\n"
							"%s%zdeq:\n"
							"\tmov eax , 1\n"
							"%s%zdend:\n"
							"\tmov [esp+%zd] , eax\n",
							node->children[1]->number*4, node->children[0]->number*4, 
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							J_EXPR_LABEL, node->expr->idnumber,
							node->number*4); 
					break;
				default:
					fprintf(stderr, "fatal: unknown expresion type: %d\n", node->expr->etype);
					abort();
			}
		}
	} else if (j_expr_is_unary(node->expr)) {
		fprintf(tree->output, "\tmov eax , [esp+%zd]\n", node->children[0]->number*4);
		
		switch (node->expr->etype) {
			case J_EXPR_NOT:
				assert(J_BOOLEAN == node->children[0]->expr->type);
				fprintf(tree->output, 
						"\txor eax , 1\n"
						"\tand eax , 1\n"
						"\tmov [esp+%zd] , eax\n", node->number*4);
				break;
			case J_EXPR_NEG:
				assert(J_BOOLEAN != node->children[0]->expr->type);
				if (J_INTEGER == node->children[0]->expr->type) {
					fprintf(tree->output, 
							"\tneg eax\n"
							"\tmov [esp+%zd] , eax\n", node->number*4);
				} else {
					fprintf(tree->output,
							"\tfld dword [esp+%zd]\n"
							"\tfchs\n"
							"\tfstp dword [esp+%zd]\n",
							node->children[0]->number*4, node->number*4);
				}
				break;
			case J_EXPR_CAST:
				if (J_BOOLEAN == node->expr->type) {
					if (J_BOOLEAN == node->children[0]->expr->type) {
						fprintf(tree->output, 
								"\tmov eax , [esp+%zd]\n"
								"\tmov [esp+%zd] , eax\n", 
								node->children[0]->number*4, node->number*4);
					} else {
						fprintf(tree->output, 
								"\tmov eax , [esp+%zd]\n"
								"\ttest eax , eax\n"
								"\tjz %s%zdcast\n"
								"\tmov eax , 1\n"
								"%s%zdcast:\n"
								"\tmov [esp+%zd] , eax\n", 
								node->children[0]->number*4, 
								J_EXPR_LABEL, node->expr->idnumber,
								J_EXPR_LABEL, node->expr->idnumber,
								node->number*4);
					}
				} else if (J_INTEGER == node->expr->type) {
					if (J_DOUBLE != node->children[0]->expr->type) {
						fprintf(tree->output, 
								"\tmov eax , [esp+%zd]\n"
								"\tmov [esp+%zd] , eax\n", 
								node->children[0]->number*4, node->number*4);
					} else {
						fprintf(tree->output,
								"\tfld dword [esp+%zd]\n" //- ladujemy double-a
								"\tfistp dword [esp+%zd]\n", //- zdejmujemy int-a
								node->children[0]->number*4, node->number*4);
					}	
				} else {
					if (J_DOUBLE != node->children[0]->expr->type) {
						fprintf(tree->output,
								"\tfild dword [esp+%zd]\n"  //- ladujemy int-a
								"\tfstp dword [esp+%zd]\n", //- zdejmujemy double-a
								node->children[0]->number*4, node->number*4);
					} else {						
						fprintf(tree->output, 
								"\tmov eax , [esp+%zd]\n"
								"\tmov [esp+%zd] , eax\n", 
								node->children[0]->number*4, node->number*4);
					}
				}
				break;
			default:
				fprintf(stderr, "fatal: unknown expresion type: %d\n", node->expr->etype);
				abort();
		}
	} else {
		fprintf(stderr, "fatal: unknown expresion type: %d\n", node->expr->etype);
		abort();
	}

	if (NULL != node->parent) { 
		//- Leniwe wyliczanie && i ||
		//- Jesli lewy wezel roztrzyga o wartosci wyrazenia, drugiej
		//- galezi nie liczymy. Trzeba pozostawiac w kodzie sprawdzenia.

		if (J_EXPR_AND == node->parent->expr->etype) {
			if (node->parent->children[0] == node) { // lewy argument
				fprintf(tree->output,
						"\tmov eax , [esp+%zd]\n"
						"\tmov ebx , 1\n"
						"\tmov [esp+%zd] , ebx\n"
						"\ttest eax , eax\n"
						"\tjz near %s%zdlazy\n", //- jak 0 to nie trzeba liczyc dalej
						node->number*4, node->parent->children[1]->number*4,
						J_EXPR_LABEL, node->parent->expr->idnumber);
			} else { //- prawy argument, zostawiamy etykiete
				fprintf(tree->output,
						"%s%zdlazy:\n", 
						J_EXPR_LABEL, node->parent->expr->idnumber);
			}
		}

		if (J_EXPR_OR == node->parent->expr->etype) {
			if (node->parent->children[0] == node) { //- lewy argument
				fprintf(tree->output,
						"\tmov eax , [esp+%zd]\n"
						"\tmov ebx , 0\n"
						"\tmov [esp+%zd] , ebx\n"
						"\ttest eax , eax\n"
						"\tjnz near %s%zdlazy\n", //- jak 1 to nie trzeba liczyc dalej
						node->number*4, node->parent->children[1]->number*4,
						J_EXPR_LABEL, node->parent->expr->idnumber);
			} else { //- prawy argument, zostawiamy etykiete
				fprintf(tree->output,
						"%s%zdlazy:\n", 
						J_EXPR_LABEL, node->parent->expr->idnumber);
			}
		}
	}
	
	//- Ok
}

void j_expr_node_generate_asm_code_idexpr(
		j_expr_tree_t               *tree,
		j_expr_node_t               *node)
{
	size_t i, depth;
	//- Chcemy poznac adres zmiennej gdzie *byla* zdefiniowana, nie gdzie *bedzie*.
	//j_vartbl_inf_t *inf = j_context_find_var_inf(curr, assign->id->number, &depth);
	j_vartbl_inf_t *inf = j_context_find_var_inf_directly(
			tree->ctx, node->expr->id->id->ctx, node->expr->id->id->number, &depth);
	assert(NULL != inf);
	
	//- Wynik na eax !!!

	if (J_ID_ONLY == node->expr->id->type) {
		fprintf(tree->output, "\tmov ecx , ebp\n");
		for (i = 0; i < depth; ++i) {
			fprintf(tree->output, "\tmov edx , [ecx]\n");
			fprintf(tree->output, "\tmov ecx , edx\n");
		}

		fprintf(tree->output,	"\tmov eax , [ecx%s%d]\n",
				inf->offset > 0 ? "+":"-", inf->offset > 0 ? inf->offset:-inf->offset);
		return;
	}

	if (J_ASSIGN == node->expr->id->type) {
		//- Na eax umieszczamy wartosc wyrazenia
		//- pozwalamy sobie na utworzenie nowego drzewa wyrazen, jest to 
		//- bezpieczne, gdyz zachowuje przyjety porzadek wyliczania 
		//- wyrazen od lewego do prawego.
		j_expr_tree_t *etree = j_new_expr_tree(
				tree->program, tree->output, tree->ctx, node->expr->id->expr);
		assert(NULL != etree);

		j_expr_tree_generate_asm_code(etree);
		j_delete_expr_tree(etree);	

		fprintf(tree->output, "\tmov ecx , ebp\n");
		for (i = 0; i < depth; ++i) {
			fprintf(tree->output, "\tmov edx , [ecx]\n");
			fprintf(tree->output, "\tmov ecx , edx\n");
		}

		//- Na eax juz jest, jeszcze zapis...
		fprintf(tree->output,	"\tmov [ecx%s%d] , eax\n",
				inf->offset > 0 ? "+":"-", inf->offset > 0 ? inf->offset:-inf->offset);
		return;
	}
	
	fprintf(tree->output, "\tmov ecx , ebp\n");
	for (i = 0; i < depth; ++i) {
		fprintf(tree->output, "\tmov edx , [ecx]\n");
		fprintf(tree->output, "\tmov ecx , edx\n");
	}

	fprintf(tree->output,	"\tmov eax , [ecx%s%d]\n",
			inf->offset > 0 ? "+":"-", inf->offset > 0 ? inf->offset:-inf->offset);

	if (J_ASSIGN_DEC == node->expr->id->type)
		fprintf(tree->output, "\tdec eax\n");
	else if (J_ASSIGN_INC == node->expr->id->type)
		fprintf(tree->output, "\tinc eax\n");
	else {
		fprintf(stderr, "fatal: unknown assign type : %d\n", node->expr->id->type);
		abort();
	}

	//- Na eax juz jest, jeszcze zapis...
	fprintf(tree->output,	"\tmov [ecx%s%d] , eax\n",
			inf->offset > 0 ? "+":"-", inf->offset > 0 ? inf->offset:-inf->offset);
}


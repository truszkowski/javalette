/**
 * @brief Wyliczanie wartosci wyrazenia do kodu asemblera.
 * @author Piotr Truszkowski
 */

#ifndef _J_EXPR_RESOLV_H_
#define _J_EXPR_RESOLV_H_

#include <stdio.h>
#include <assert.h>
#include "types.h"
#include "program.h"
#include "context.h"

typedef enum {
	J_EXPR_NODE_WAIT,
	J_EXPR_NODE_READY,
	J_EXPR_NODE_DONE
} j_expr_node_status_t;

/**
 * @brief Struktura wezla
 */
typedef struct j_expr_node_t {
	j_expr_t                      *expr;
	size_t                         number;
	size_t                         count;
	j_expr_node_status_t           status;
	struct j_expr_node_t          *parent;
	struct j_expr_node_t          *left;
	struct j_expr_node_t          *right;
	struct j_expr_node_t          *children[];
} j_expr_node_t;

/**
 * @brief Struktura drzewa
 */
typedef struct j_expr_tree_t {
	j_expr_node_t                 *root;
	j_program_t                   *program;
	FILE                          *output;
	j_context_t                   *ctx;
	size_t                         size;
} j_expr_tree_t;

/**
 * @brief Utworzenie drzewa
 */
j_expr_tree_t *j_new_expr_tree(
		j_program_t                 *program,
		FILE                        *output,
		j_context_t                 *ctx,
		j_expr_t                    *expr);

/**
 * @brief Zwolnienie drzewa
 */
void j_delete_expr_tree(
		j_expr_tree_t               *tree);

/**
 * @brief Utworzenie wezla
 */
j_expr_node_t *j_new_expr_node(
		j_expr_tree_t               *tree,
		j_expr_node_t               *parent,
		j_expr_t                    *expr);

/**
 * @brief Zwolnienie wezla
 */
void j_delete_expr_node(
		j_expr_node_t               *node);

/**
 * @brief Generowanie kodu asemblera
 */
void j_expr_tree_generate_asm_code(
		j_expr_tree_t               *tree);

/**
 * @brief Generowanie kodu asemblera
 */
void j_expr_node_generate_asm_code(
		j_expr_tree_t               *tree,
		j_expr_node_t               *node);

/**
 * @brief Generowanie kodu asemblera
 */
void j_expr_node_generate_asm_code_idexpr(
		j_expr_tree_t               *tree,
		j_expr_node_t               *node);

#endif

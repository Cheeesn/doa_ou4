#ifndef __ARRAY_2D_H
#define __ARRAY_2D_H

#include <stdbool.h>
#include "util.h"

/*
 * Declaration of a generic 2D array for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University. The array stores void pointers, so it can be used to
 * store all types of values. After use, the function array_kill must
 * be called to de-allocate the dynamic memory used by the array
 * itself. The de-allocation of any dynamic memory allocated for the
 * element values is the responsibility of the user of the array,
 * unless a kill_function is registered in array_create.
 *
 * An element value of NULL is considered to be "no" value.
 *
 * Authors: Niclas Borlin (niclas@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   v1.0  2018-01-28: First public version.
 *   v1.1  2018-04-03: Moved freehandler to last in create parameter list.
 *   v1.2  2024-05-10: Added print_internal.
 */

// ==========PUBLIC DATA TYPES============

// List type.
typedef struct array_2d array_2d;

// ==========DATA STRUCTURE INTERFACE==========

/**
 * array_2d_create() - Create an array without values.
 * @lo1: low index limit for first dimension.
 * @hi1: high index limit for first dimension.
 * @lo2: low index limit for second dimension.
 * @hi2: high index limit for second dimension.
 * @kill_func: A pointer to a function (or NULL) to be called to
 *             de-allocate memory on remove/kill.
 *
 * The index limits are inclusive, i.e. all indices i such that low <=
 * i <= high are defined.
 *
 * Returns: A pointer to the new array, or NULL if not enough memory
 * was available.
 */
array_2d *array_2d_create(int lo1, int hi1, int lo2, int hi2,
                          kill_function kill_func);

/**
 * array_2d_low() - Return the low index limit for the array.
 * @a: array to inspect.
 * @d: dimension number, 1 or 2.
 *
 * Returns: The low index limit for dimension number d.
 */
int array_2d_low(const array_2d *a, int d);

/**
 * array_2d_high() - Return the high index limit for the array.
 * @a: array to inspect.
 * @d: dimension number, 1 or 2.
 *
 * Returns: The high index limit for dimension number d.
 */
int array_2d_high(const array_2d *a, int d);

/**
 * array_2d_inspect_value() - Inspect a value at a given array position.
 * @a: array to inspect.
 * @i: first index of position to inspect.
 * @j: second index of position to inspect.
 *
 * Returns: The element value at the specified position. The result is
 *          undefined if no value are stored at that position.
 */
void *array_2d_inspect_value(const array_2d *a, int i, int j);

/**
 * array_2d_has_value() - Check if a value is set at a given array position.
 * @a: array to inspect.
 * @i: first index of position to inspect.
 * @j: second index of position to inspect.
 *
 * Returns: True if a value is set at the specified position, otherwise false.
 */
bool array_2d_has_value(const array_2d *a, int i, int j);

/**
 * array_2d_set_value() - Set a value at a given array position.
 * @a: array to modify.
 * @v: value to set element to, or NULL to clear value.
 * @i: first index of position to inspect.
 * @j: second index of position to inspect.
 *
 * If the old element value is non-NULL, calls kill_func if it was
 * specified at array creation.
 *
 * Returns: Nothing.
 */
void array_2d_set_value(array_2d *a, void *v, int i, int j);

/**
 * array_2d_kill() - Return memory allocated by array.
 * @a: array to kill.
 *
 * Iterates over all elements. If kill_func was specified at array
 * creation, calls it for every non-NULL element value.
 *
 * Returns: Nothing.
 */
void array_2d_kill(array_2d *a);

/**
 * array_2d_print() - Iterate over the array element and print their values.
 * @a: Array to inspect.
 * @print_func: Function called for each non-NULL element.
 *
 * Iterates over each position in the array. Calls print_func for each
 * non-NULL value.
 *
 * Returns: Nothing.
 */
void array_2d_print(const array_2d * l, inspect_callback print_func);

/**
 * array_2d_print_internal() - Print the arrays internal structure.
 * @a: Array to inspect.
 * @print_func: Function called for each element value.
 * @desc: String with a description/state of the array, or NULL for no description.
 * @indent_level: Indentation level, 0 for outermost
 *
 * Iterates over the array and outputs dot code that shows the internal
 * structure of the array.  The dot code can be visualized by
 * Graphviz.
 *
 * On linux system, the output can be parsed by the dot program, e.g.
 *
 *   <array_program> | dot -Tsvg > /tmp/dot.svg; firefox /tmp/dot.svg
 *
 * where <array_program> is the name of the executable
 *
 * The output may also be possible to visualize online on
 * https://dreampuf.github.io/GraphvizOnline/ or google "graphviz
 * online".
 *
 * For documention of the dot language, see graphviz.org.
 *
 * Returns: Nothing.
 */
void array_2d_print_internal(const array_2d *a, inspect_callback print_func, const char *desc,
                             int indent_level);

#endif

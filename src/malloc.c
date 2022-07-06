/**
 * @file malloc.c
 * @author tmatis (tmatis@student.42.fr)
 * @brief The malloc functions
 * @date 2022-06-06
 *
 */

#include <unistd.h>
#include "malloc.h"
#include <stdio.h>
#include <string.h>

pthread_mutex_t g_malloc_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief Custom malloc function
 * This function is named ft_malloc or malloc
 * @param size size of the block
 * @return void* The allocated memory or NULL if failed
 */
void *_malloc(size_t size)
{
	size = ALLIGN_16(size);

	if (size < MINIMAL_SIZE)
		size = MINIMAL_SIZE;

	area_ptr area = get_or_create_area();
	if (area == NULL)
		return NULL;

	block_ptr *root = get_proper_root(size);

	if (!root)
		return handle_off_map(size);

	block_ptr best_fit = find_best_fit(size, root);
	if (!best_fit)
		return handle_off_map(size);

	delete_free_block(best_fit, root);
	if (get_block_size(best_fit) - size > get_minimal_size(best_fit, area))
	{
		block_ptr new_block = split_block(best_fit, size, find_area_end(area, best_fit));
		insert_free_block(new_block, root);
	}
	set_allocated(best_fit);
	return get_block_data(best_fit);
}

/**
 * @brief Custom calloc function
 * This function is named ft_calloc or calloc
 * @param nmemb number of elements
 * @param size size of the element
 * @return void* The allocated memory or NULL if failed
 */
void *_calloc(size_t nmemb, size_t size)
{
	size_t total_size = nmemb * size;
	if (total_size == 0)
		total_size = 1;
	void *data = _malloc(total_size);
	if (data)
		ft_memset(data, 0, total_size);
	return data;
}

/**
 * @brief Custom free function
 * This function is named ft_free or free
 * @param data the data ptr to free
 */
void _free(void *data)
{
	if ((size_t)data % 16 != 0)
	{
		error_write("free: invalid pointer");
		return ;
	}
	if (data == NULL)
		return;

	area_ptr area = get_or_create_area();
	if (area == NULL)
		return;
	block_ptr block = get_block_from_data(data);

	if (!is_allocated(block))
		return;

	if (is_off_map(block, area))
		return remove_off_map_block(block);

	set_free(block);

	block_ptr *root = find_proper_root(area, block);
	if (!root)
	{
		error_write("free: failed to find root");
		return;
	}

	block_ptr new_block = unfrag_block(block, find_area_end(area, block), root);
	insert_free_block(new_block, root);
}

/**
 * @brief Custom realloc function
 * This function is named ft_realloc or realloc
 * this function try to expand the block or allocate a new block
 * and copy the data to the new block
 * @param ptr the block to reallocate
 * @param size the new size of the block
 * @return void* The allocated memory or NULL if failed
 */
void *_realloc(void *ptr, size_t size)
{
	if ((size_t)ptr % 16 != 0)
	{
		error_write("realloc: invalid pointer");
		return NULL;
	}
	if (size == 0)
	{
		_free(ptr);
		return NULL;
	}

	size = ALLIGN_16(size);

	if (size < MINIMAL_SIZE)
		size = MINIMAL_SIZE;

	if (ptr == NULL)
		return _malloc(size);

	block_ptr block = get_block_from_data(ptr);
	size_t old_size = get_block_size(block);

	if (size == old_size)
		return ptr;

	area_ptr area = get_or_create_area();
	if (area == NULL)
		return NULL;

	if (!is_off_map(block, area))
	{
		if (size < old_size && !shrink_block(block, size, area))
			// if we succeed to shrink the block, we return the data
			return (ptr);

		if (size > old_size && !extend_block(block, size, area))
			// if we succeed to extend the block, we return the data
			return (ptr);
	}
	// select lower size
	size_t to_copy = old_size < size ? old_size : size;
	void *new_ptr = _malloc(size);
	if (new_ptr)
	{
		ft_memcpy(new_ptr, ptr, to_copy);
		_free(ptr);
	}
	return new_ptr;
}

void EXPORT *MALLOC_NAME(size_t size)
{
#ifdef BONUS
	t_config const *config = get_config();
	pthread_mutex_lock(&g_malloc_mutex);
#endif
	void *data = _malloc(size);
#ifdef BONUS
	if (config->verbose)
	{
		ft_putstr("malloc: ");
		if (data)
			dump_block(get_block_from_data(data));
		else
			ft_putstr("NULL\n");
	}
	pthread_mutex_unlock(&g_malloc_mutex);
#endif
	return data;
}

void EXPORT *CALLOC_NAME(size_t nmemb, size_t size)
{
#ifdef BONUS
	t_config const *config = get_config();
	pthread_mutex_lock(&g_malloc_mutex);

	if (config->verbose)
	{
		ft_putstr("calloc: ");
	}
#endif
	void *data = _calloc(nmemb, size);
#ifdef BONUS
	pthread_mutex_unlock(&g_malloc_mutex);
#endif
	return data;
}

void EXPORT *REALLOC_NAME(void *ptr, size_t size)
{
#ifdef BONUS
	t_config const *config = get_config();
	pthread_mutex_lock(&g_malloc_mutex);
#endif
	void *data = _realloc(ptr, size);
#ifdef BONUS
	if (config->verbose)
	{
		ft_putstr("realloc: ");
		if (data)
			dump_block(get_block_from_data(data));
		else
			ft_putstr("NULL\n");
	}
	pthread_mutex_unlock(&g_malloc_mutex);
#endif
	return data;
}

void EXPORT FREE_NAME(void *data)
{
#ifdef BONUS
	t_config const *config = get_config();
	pthread_mutex_lock(&g_malloc_mutex);
#endif
#ifdef BONUS
	if (config->verbose)
	{
		ft_putstr("free: ");
		if (data)
			dump_block(get_block_from_data(data));
		else
			ft_putstr("NULL\n");
	}
#endif
	_free(data);
#ifdef BONUS
	pthread_mutex_unlock(&g_malloc_mutex);
#endif
}
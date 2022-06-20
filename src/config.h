#ifndef CONFIG_H
#define CONFIG_H

typedef struct s_config
{
	char verbose;
} t_config;

t_config const *get_config(void);
t_config parse_config(char const* str);


#endif
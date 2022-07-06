#include "config.h"
#include "malloc.h"
#include <stdlib.h>

#ifdef BONUS

t_config parse_config(char const *str)
{
	char buf[1024];
	char const *ptr = str;
	t_config config = {0};

	// the format is flag1, flag2, flag3, ...
	while (*ptr != '\0')
	{
		char *end;

		// skip spaces
		while (*ptr == ' ' || *ptr == ',' || *ptr == '\t')
			ptr++;
		if (*ptr == '\0')
			break;

		// get the value check for buffer overflow
		end = buf;
		while (*ptr != ' ' && *ptr != ',' && *ptr != '\t' && *ptr != '\0')
		{
			if (end - buf >= 1023)
				break;
			*end++ = *ptr++;
		}
		*end = '\0';
		// strcmp buf with the flag name
		if (ft_strcmp(buf, "verbose") == 0)
		{
			ft_putstr("verbose\n");
			config.verbose = 1;
		}
		if (ft_strcmp(buf, "visual") == 0)
		{
			ft_putstr("verbose\n");
			config.visual = 1;
		}
	}
	return (config);
}

t_config const *get_config(void)
{
	static t_config store = {0};
	static char is_init = 0;

	if (!is_init)
	{
		char const *config = getenv("MALLOC_CONFIG");
		if (config == NULL)
			config = "";
		store = parse_config(config);
		is_init = 1;
		if (store.visual)
			launch_thread();
	}
	return (&store);
}

#endif
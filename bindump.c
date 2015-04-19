#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <getopt.h>

#define TRUE 1
#define FALSE 0

char *filename = NULL;
unsigned int skip = 0;
unsigned int length = UINT_MAX;
unsigned int count = 8;
unsigned int show_ascii = FALSE;
unsigned int show_hex = FALSE;
unsigned int show_dec = FALSE;
unsigned int split = TRUE;
unsigned int show_offset = FALSE;
unsigned int show_ascii_buffer = FALSE;

void
show_help(void)
{
	printf("Usage: bindump [-snAHDoavh] filename\n");
	printf("       bd [-snHADoavh] filename\n");
	printf("Arguments:\n");
	printf("\t-s\tSkip count of bytes\n");
	printf("\t-l\tLength of bytes to show\n");
	printf("\t-c\tCount of bytes in string\n");
	printf("\t-A\tShow ASCII values near the binary\n");
	printf("\t-H\tShow hex values near the binary\n");
	printf("\t-D\tShow dec values near the binary\n");
	printf("\t-S\tDo not split binary bytes\n");
	printf("\t-o\tShow offsets on start of every string\n");
	printf("\t-a\tShow ASCII string after binary bytes\n");
	printf("\t-v\tShow version info\n");
	printf("\t-h\tShow this help\n");
	return;
}

void
show_version(void)
{
	printf("%s %s\n", PACKAGE, VERSION);
	return;
}

void
get_binary_string(char *buffer, char symbol)
{
	int i = 0;
	for(i = 0; i <= 8; i++)
		buffer[8-i] = (1 & (symbol >> i)) ? '1' : '0';
	return;
}

void
parse_args(int argc, char **argv)
{
	unsigned int i = 0;
	unsigned int j = 0;

	for(i = 1; i < argc; i++)
	{
		char *arg = argv[i];
		if(arg[0] == '-')
		{
			j = 0;
			while(arg[++j] != '\0')
			{
				switch(arg[j])
				{
				case 's':
					if(++i >= argc)
					{
						fprintf(stderr, "Option -s requires a value\n");
						exit(1);
					}
					skip = atoi(argv[i]);
					if((skip <= 0) && argv[i][0] != '0')
					{
						fprintf(stderr, "Bad value for -s given: %s\n",
							argv[i]);
						exit(1);
					}
					goto NEXT_ARG;
					break;

				case 'l':
					if(++i >= argc)
					{
						fprintf(stderr, "Option -l requires a value\n");
						exit(1);
					}
					length = atoi(argv[i]);
					if((length <= 0) && argv[i][0] != '0')
					{
						fprintf(stderr, "Bad value for -l given: %s\n",
								argv[i]);
						exit(1);
					}
					goto NEXT_ARG;
					break;

				case 'c':
					if(++i >= argc)
					{
						fprintf(stderr, "Option -c requires a value\n");
						exit(1);
					}
					count = atoi(argv[i]);
					if((count <= 0) && argv[i][0] != '0')
					{
						fprintf(stderr, "Bad value for -c given: %s\n",
								argv[i]);
						exit(1);
					}
					break;

				case 'A':
					show_ascii = TRUE;
					break;

				case 'H':
					show_hex = TRUE;
					break;

				case 'D':
					show_dec = TRUE;
					break;

				case 'S':
					split = FALSE;
					break;

				case 'o':
					show_offset = TRUE;
					break;

				case 'a':
					show_ascii_buffer = TRUE;
					break;

				case 'v':
					show_version();
					exit(0);
					break;

				case 'h':
					show_help();
					exit(0);
					break;

				default:
					fprintf(stderr, "Unknown option -%c\n", arg[j]);
					exit(1);
					break;
				}
				continue;
			}
		} else {
			filename = arg;
		}

	NEXT_ARG:
		continue;
	}

	return;
}

int
main(int argc, char **argv)
{
	unsigned int offset = 0;
	unsigned int i = 0;
	int c = 0;
	char *buffer = malloc(sizeof(char) * 8);
	char *ascii_buffer = malloc(sizeof(char) * count);

	parse_args(argc, argv);

	if(filename != NULL)
	{
		fclose(stdin);
		stdin = fopen(filename, "r");
		if(stdin == NULL)
		{
			printf("Unable to open file %s\n", filename);
			exit(1);
		}
	}

	while(skip-- > 0)
		fgetc(stdin);

	while((c != EOF) && (length > 0))
	{
		if(show_offset)
			fprintf(stdout, "0x%04x: ", offset);

		for(i = 0; i < count; i++)
		{
			ascii_buffer[i] = ' ';
			if(length != 0)
			{
				c = fgetc(stdin);
				if(c == EOF)
					break;
				ascii_buffer[i] = (isalnum(c)) ? c : '.';

				get_binary_string(buffer, c);
				fprintf(stdout, "%s", buffer);
				if(show_ascii)
					fprintf(stdout, "[%c]", isalnum(c) ? c : '.');
				if(show_hex)
					fprintf(stdout, "[0x%02x]", c);
				if(show_dec)
					fprintf(stdout, "[%03d]", c);

				if(split)
					fprintf(stdout, " ");

				offset++;
				length--;
			}
		}

		if(show_ascii_buffer)
			fprintf(stdout, "%s", ascii_buffer);

		fprintf(stdout, "\n");
	}

	free(buffer);
	free(ascii_buffer);
	return 0;
}

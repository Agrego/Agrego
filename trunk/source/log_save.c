#include "headers/xlib.h"
#include "headers/log_save.h"
#include "headers/xiniparser.h"

/**
 * @param {level} level Message log level
 * @param {format} format format of the message as used for printf
 * @param {...} Variables used in the message
 */
int	log_write(int level, const char *format, ...)
{
	FILE	*log_file;
	char	message[MAX_LINE_LEN];
	va_list	args;

	log_file = NULL;
	if (!log_check_level(level))
	{
		return 0;
	}

	if (xiniparser_getstring("Logs:use_syslog", 0))
	{
		openlog(SYSLOG_IDENT, LOG_PID, LOG_DAEMON);
		va_start(args, format);
		vsyslog(level, format, args);
		va_end(args);
		closelog();
		return 0;
	}

	if ((log_file = log_open_file()) == NULL)
	{
		return -1;
	}

	va_start(args, format);
	if ((vsprintf(message, format, args)) > 0)
	{
		log_write_in(log_file, message);
	}

	va_end(args);
	xfclose(log_file);
	return 0;
}

/**
 * fopen It doesn't use xlib because the error have not to be displayed into log but into syslog, else we will have a recursive issue
 * @return file descriptor positioned at the end of the file
**/
FILE 	*log_open_file()
{
	FILE 	*log_file;
	char	*filename;

	filename = xiniparser_getstring("Logs:filename", "/var/log/agrego/main.log");
	log_file = fopen(filename, "a");
	if (log_file == NULL)
	{
		/* log into syslog ? can't log in file %s: %s, file, strerror(errno)*/
		perror("Log file open");
		return NULL;
	}
	return log_file;
}

/**
 * Write message into log file
 * @param {log_file} the file where log will be writed
 * @param {message} message to write
**/
void	log_write_in(FILE *log_file, char *message)
{
	time_t		now;
	struct tm	*ts;
	char		buf[80];

	now = time(NULL);
	ts = localtime(&now);

	strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", ts);
	if (fprintf(log_file, "%s %d %s\n", buf, getpid(), message) < 0)
	{
		/* log into syslog ? can't log in file %s: %s, file, strerror(errno)*/
		perror("can't write on log file");
		/* Sure ??? */
		exit(errno);
	}
	fflush(log_file);
}

/**
 * @return True if level have to be written false either
**/
inline int	log_check_level(int level)
{
	return level <= xiniparser_getint("Logs:level", LOG_ERR);
}

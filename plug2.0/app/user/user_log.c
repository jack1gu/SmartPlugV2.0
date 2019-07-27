#include "user_common.h"

#define LOG_PREFIX_BUF_SIZE 	(100)
#define LOG_BUF_SIZE 			(300)


//��־Ĭ��INFO����
static UINT uiLogLevel = LOGOUT_INFO;
static CHAR *g_pcLogBuf = NULL;

VOID LOG_LogInit( VOID )
{
	//printf("LOG_LogInit start.\r\n");

 	g_pcLogBuf = (CHAR*)malloc( LOG_PREFIX_BUF_SIZE + LOG_BUF_SIZE + 5 );
	if ( NULL == g_pcLogBuf )
	{
		printf("[ERROR][%s:%d][%s]# malloc failed, Free heap:%d.",
				__FILE__, __LINE__, __func__, system_get_free_heap_size());
		while(1);
	}
	//printf("LOG_LogInit over.\r\n");
}

VOID LOG_SetLogLevel( UINT uiLevel )
{
	if( uiLevel >= LOGOUT_DEBUG && uiLevel <= LOGOUT_NONE )
	{
		uiLogLevel = uiLevel;
	}
	else
	{
		printf("LOG_SetLogLevel failed, uiLevel: %d.\r\n", uiLevel);
	}
}


VOID LOG_Logout(UINT uiLevel, CHAR *pcFileName, INT iLine, CHAR *pcfunc, CHAR *pcFamt, ...)
{
	va_list Arg;
	CHAR *pcPos = NULL;
	CHAR *pcBuf = NULL;

	pcPos = pcBuf = g_pcLogBuf;

	if ( NULL == pcFamt)
	{
		return;
	}

	if ( uiLevel < uiLogLevel )
	{
		return;
	}

	switch ( uiLevel )
	{
	    case LOGOUT_ERROR :
			pcPos += snprintf(pcBuf, LOG_PREFIX_BUF_SIZE, "[%s][%s:%d][%s]# ", "ERROR", pcFileName, iLine, pcfunc );
			break;
	    case LOGOUT_INFO :
			pcPos += snprintf(pcBuf, LOG_PREFIX_BUF_SIZE, "[%s][%s:%d][%s]# ", "INFO", pcFileName, iLine, pcfunc );
	        break;
	    case LOGOUT_DEBUG :
			pcPos += snprintf(pcBuf, LOG_PREFIX_BUF_SIZE, "[%s][%s:%d][%s]# ", "DEBUG", pcFileName, iLine, pcfunc );
	        break;
	    default:
	        return;
	}

	va_start(Arg, pcFamt);
	pcPos += vsnprintf(pcPos, LOG_BUF_SIZE, pcFamt, Arg);
	va_end(Arg);


	pcPos += sprintf(pcPos,"\r\n");
	printf(pcBuf);

	//�ж������־�Ƿ����ض�
	if ( (pcPos - pcBuf) > (LOG_BUF_SIZE + LOG_PREFIX_BUF_SIZE))
	{
		printf("[%s][%s:%d][%s]# Log too large has been cut off.\r\n", "ERROR", __FILE__, __LINE__, __func__);
	}
	return;
}

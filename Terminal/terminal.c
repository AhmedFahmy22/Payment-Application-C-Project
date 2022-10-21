#include <stdio.h>
#include <time.h>
#include "../STD_Types.h"
#include "../Card/card.h"
#include "terminal.h"

EN_terminal_Error getTransactionDate(ST_terminalData *termData)
{
  uint8 u8CntrLoc=0;
  uint8 *pu8;
  pu8=&(termData->transactionDate[0]);
  EN_terminal_Error enuErrorStateLoc = OK_TERM;
  if(termData=NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_TERM;
  }
  else
  {
    time_t time0;
    time0= time(NULL);
    struct tm strTimeLoc = *localtime(&time0);

    while(u8CntrLoc<11)
    {
      switch(u8CntrLoc)
      {
        case 2:
        case 5:
        pu8[u8CntrLoc] = '/';
        break;
        case 10:
        pu8[u8CntrLoc] = '\0';
        break;
        default:
        pu8[u8CntrLoc] = '0';
        break;
      }
      u8CntrLoc++;
    }

    pu8[0]  +=   strTimeLoc.tm_mday/10;
    pu8[1]  +=   strTimeLoc.tm_mday%10;
    pu8[3]  +=   (strTimeLoc.tm_mon+1)/10;
    pu8[4]  +=   (strTimeLoc.tm_mon+1)%10;
    pu8[6]  +=   (strTimeLoc.tm_year+1900)/1000 ;
    pu8[7]  +=   ((strTimeLoc.tm_year+1900)/100)%10 ;
    pu8[8]  +=   ((strTimeLoc.tm_year+1900)/10)%10 ;
    pu8[9]  +=   (strTimeLoc.tm_year+1900)%10 ;

	  for(u8CntrLoc=0;u8CntrLoc<=10;u8CntrLoc++)
    {
		if(pu8[u8CntrLoc]=='\0')
	    {
		  break;
	    }
	    else if((pu8[u8CntrLoc]<'0')||(pu8[u8CntrLoc]>'9'))
        {
          if((u8CntrLoc!=2)&&(u8CntrLoc!=5))
		  {
		    u8CntrLoc=11;
		    break;
		  }
        }
    }
    if(u8CntrLoc!=10||(pu8[2]!='/')||(pu8[5]!='/'))
    {
      enuErrorStateLoc = WRONG_DATE;
    }
  }
  return enuErrorStateLoc;
}

EN_terminal_Error isCardExpired(ST_cardData cardData, ST_terminalData termData)
{
  EN_terminal_Error enuErrorStateLoc = OK_TERM;
  /*transform Date to months*/
  uint16 cardDataMonthLoc,termDataMonthLoc;

  cardDataMonthLoc = (cardData.cardExpirationDate[0]-'0')*10+(cardData.cardExpirationDate[1]-'0')+((cardData.cardExpirationDate[3]-'0')*10+(cardData.cardExpirationDate[4]-'0'))*12;
  termDataMonthLoc = (termData.transactionDate[3]-'0')*10+(termData.transactionDate[4]-'0')+((termData.transactionDate[8]-'0')*10+(termData.transactionDate[9]-'0'))*12;
  if(cardDataMonthLoc<termDataMonthLoc)
  {
    enuErrorStateLoc = WRONG_DATE;
  }
  return enuErrorStateLoc;
}

EN_terminal_Error isValidCardPan(ST_cardData *cardData)
{
  uint16 u16LuhnSumLoc=0,u8PanSizeLoc=0,u8CntrLoc;
  EN_terminal_Error enuErrorStateLoc = OK_TERM;
  while(cardData->primaryAccountNumber[u8PanSizeLoc]!='\0')
  {
    u8PanSizeLoc++;
  }
  for(u8CntrLoc=u8PanSizeLoc-2;u8CntrLoc>=0;u8CntrLoc-=2)
  {
    u16LuhnSumLoc = u16LuhnSumLoc + (cardData->primaryAccountNumber[u8CntrLoc]-'0')*2;
	if(u8CntrLoc==1||u8CntrLoc==0)
	{
		break;
	}
  }

  if(u16LuhnSumLoc%10!=0)
  {
    enuErrorStateLoc = INVALID_CARD;
  }
  return enuErrorStateLoc;
}

EN_terminal_Error getTransactionAmount(ST_terminalData *termData)
{
  float32 *pf32 = &(termData->transAmount);
  EN_terminal_Error enuErrorStateLoc = OK_TERM;
  if(termData=NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_TERM;
  }
  else
  {
    printf("Enter the transaction amount: ");
    scanf("%f",pf32);

    if(*pf32<=0)
    {
      enuErrorStateLoc = INVALID_AMOUNT;
    }
  }
  return enuErrorStateLoc;
}

EN_terminal_Error isBelowMaxAmount(ST_terminalData *termData)
{
  float32 *pf32MTA = &(termData->maxTransAmount);
  float32 *pf32TA = &(termData->transAmount);
  EN_terminal_Error enuErrorStateLoc = OK_TERM;
  if(termData=NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_TERM;
  }
  else
  {
    if((*pf32TA)>(*pf32MTA))
    {
      enuErrorStateLoc = EXCEED_MAX_AMOUNT;
    }
  }
  return enuErrorStateLoc;
}

EN_terminal_Error setMaxAmount(ST_terminalData *termData)
{
  float32 *pf32 = &(termData->maxTransAmount);
  EN_terminal_Error enuErrorStateLoc = OK_TERM;

  if(termData=NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_TERM;
  }
  else
  {
    printf("Enter the maximum transaction amount: ");
    scanf("%f",pf32);

    if(*pf32<=0)
    {
      enuErrorStateLoc = INVALID_MAX_AMOUNT;
    }
  }
  return enuErrorStateLoc;
}

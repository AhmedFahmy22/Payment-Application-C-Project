#include <stdio.h>
#include "../STD_Types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "../Server/server.h"

void appStart(void)
{
  uint8 u8AccountIndxLoc;
  ST_cardData strCardDataLoc ;
  ST_terminalData strTermDataLoc;
  ST_transaction strTransDataLoc;

  transactionArrayInit();

    /***** Card *****/
  EN_cardError enuErrorStateLoc=WRONG_NAME;
  while(enuErrorStateLoc!=OK_CARD)
  {
    enuErrorStateLoc = getCardHolderName(&strCardDataLoc);
    if(enuErrorStateLoc==OK_CARD)
    {
      enuErrorStateLoc=WRONG_EXP_DATE;
      while(enuErrorStateLoc!=OK_CARD)
      {
        enuErrorStateLoc = getCardExpiryDate(&strCardDataLoc);
        if(enuErrorStateLoc==OK_CARD)
        {
          enuErrorStateLoc=WRONG_PAN;
          while(enuErrorStateLoc!=OK_CARD)
          {
            enuErrorStateLoc = getCardPAN(&strCardDataLoc);
            if(enuErrorStateLoc!=OK_CARD)
            {
              printf("Wrong PAN, try again!\n");
            }
          }
        }
        else
        {
          printf("Wrong Date, try again!\n");
        }
      }
    }
    else
    {
      printf("Wrong Name, try again!\n");
    }
  }
  printf("**Data Saved Successfully**\n");
	
    /***** Terminal *****/
  enuErrorStateLoc = (EN_terminal_Error) enuErrorStateLoc;

  enuErrorStateLoc = INVALID_MAX_AMOUNT;
  while(enuErrorStateLoc==INVALID_MAX_AMOUNT)         /* SET MAXIMUM AMOUNT */
  {
    enuErrorStateLoc = setMaxAmount(&strTermDataLoc);
  }

  enuErrorStateLoc = WRONG_DATE;
  while(enuErrorStateLoc!=OK_TERM)
  {
    enuErrorStateLoc = getTransactionDate(&strTermDataLoc);
    if(enuErrorStateLoc==OK_TERM)
    {
      enuErrorStateLoc = isCardExpired(strCardDataLoc,strTermDataLoc);
      if(enuErrorStateLoc==OK_TERM)
      {
        enuErrorStateLoc = isValidCardPan(&strCardDataLoc);
        if(enuErrorStateLoc==OK_TERM)
        {
          enuErrorStateLoc = INVALID_AMOUNT;
          while(enuErrorStateLoc!=OK_TERM)
          {
            enuErrorStateLoc = getTransactionAmount(&strTermDataLoc);
            if(enuErrorStateLoc==OK_TERM)
            {
              enuErrorStateLoc = isBelowMaxAmount(&strTermDataLoc);
              if(enuErrorStateLoc==EXCEED_MAX_AMOUNT)
              {
                printf("**Transaction Exceeded Maximum Amount**\n");
                goto End;
              }
            }
            else
            {
              printf("Invalid Amount, try again!\n");
            }
          }
        }
        else
        {
          printf("**Invalid Card PAN**\n");
          goto End;
        }
      }
      else
      {
        printf("**Card Expired**\n");
        goto End;
      }
    }
    else
    {
      printf("Wrong Date, try again!\n");
    }
  }
  printf("**Transaction Data Saved Successfully**\n");

    /***** Server *****/
  enuErrorStateLoc = (EN_serverError) enuErrorStateLoc;
  enuErrorStateLoc = isValidAccount(&strCardDataLoc,&u8AccountIndxLoc);
  if(enuErrorStateLoc == OK_SERV)
  {
    if(astrucDataBase[u8AccountIndxLoc].accountState==ALLOWED)
	{
		enuErrorStateLoc = isAmountAvailable(&strTermDataLoc,&astrucDataBase[u8AccountIndxLoc]);
		if(enuErrorStateLoc==OK_SERV)
		{
		  strTransDataLoc.cardHolderData = strCardDataLoc;
		  strTransDataLoc.terminalData = strTermDataLoc;
		  printf("\nBefore: %0.2f\n",astrucDataBase[2].balance);
		  strTransDataLoc.transState = recieveTransactionData(&strTransDataLoc);
		  printf("\nAfter: %0.2f\n",astrucDataBase[2].balance);
		}
		else
		{
		  printf("**Decline: Low Balance**\n");
		}
		}
	else
	{
		printf("**DECLINE: Transactions Blocked for Card**\n");
	}
	
  }
  else
  {
    printf("**Decline: Invalid Account**\n");
  }
  End:;
}

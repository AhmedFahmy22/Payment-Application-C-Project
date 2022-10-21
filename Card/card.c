#include "../STD_Types.h"
#include <stdio.h>
#include "card.h"

EN_cardError getCardHolderName(ST_cardData *cardData)
{
  EN_cardError enuErrorStateLoc = OK_CARD;
  if(cardData == NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_CARD;
  }
  else
  {
    uint8 u8CntrLoc;

	printf("Enter the name: ");
    scanf("%[^\n]%*c",cardData->cardHolderName);

    for(u8CntrLoc=0;u8CntrLoc<=24;u8CntrLoc++)
    {
	  if(cardData->cardHolderName[u8CntrLoc]=='\0')
	  {
		  break;
	  }
	  else if(cardData->cardHolderName[u8CntrLoc]<'a'||cardData->cardHolderName[u8CntrLoc]>'z')
      {
		if(cardData->cardHolderName[u8CntrLoc]<'A'||cardData->cardHolderName[u8CntrLoc]>'Z')
        {
          if(cardData->cardHolderName[u8CntrLoc]!=' ')
		  {
			 u8CntrLoc=25;
			 break;
		  }
        }
      }
    }
    if((u8CntrLoc<20)||(u8CntrLoc>24))
    {
      enuErrorStateLoc = WRONG_NAME;
    }
  }
  return enuErrorStateLoc;
}

EN_cardError getCardExpiryDate(ST_cardData *cardData)
{
  EN_cardError enuErrorStateLoc = OK_CARD;
  if(cardData == NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_CARD;
  }
  else
  {
    uint8 u8CntrLoc;
	printf("Enter the date(MM/YY): ");
    scanf("%s",cardData->cardExpirationDate);

    for(u8CntrLoc=0;u8CntrLoc<=5;u8CntrLoc++)
    {
      if(cardData->cardExpirationDate[u8CntrLoc]=='\0')
	  {
		  break;
	  }
	  else if(((cardData->cardExpirationDate[u8CntrLoc]<'0')||(cardData->cardExpirationDate[u8CntrLoc]>'9'))&&u8CntrLoc!=2)
      {
        u8CntrLoc=6;
		break;
      }
    }
    if((u8CntrLoc!=5) || (cardData->cardExpirationDate[2] != '/'))
    {
      enuErrorStateLoc = WRONG_EXP_DATE;
    }

    if(((cardData->cardExpirationDate[0]=='0')&&(cardData->cardExpirationDate[1]=='0'))||((cardData->cardExpirationDate[3]=='0')&&(cardData->cardExpirationDate[4]=='0')))
    {
      enuErrorStateLoc = WRONG_EXP_DATE;
    }
  }
  return enuErrorStateLoc;
}

EN_cardError getCardPAN(ST_cardData *cardData)
{
  EN_cardError enuErrorStateLoc = OK_CARD;
  if(cardData == NULL_PTR)
  {
    enuErrorStateLoc = NULL_POINTER_CARD;
  }
  else
  {
    uint8 u8CntrLoc;
	printf("Enter the PAN: ");
    scanf("%s",cardData->primaryAccountNumber);

    for(u8CntrLoc=0;u8CntrLoc<=19;u8CntrLoc++)
    {
      if(cardData->primaryAccountNumber[u8CntrLoc]=='\0')
	  {
		  break;
	  }
	  else if((cardData->primaryAccountNumber[u8CntrLoc]<'0')||(cardData->primaryAccountNumber[u8CntrLoc]>'9'))
      {
        u8CntrLoc=20;
		break;
      }
    }
    if((u8CntrLoc<16)||(u8CntrLoc>19))
    {
      enuErrorStateLoc = WRONG_PAN;
    }
  }
  return enuErrorStateLoc;
}

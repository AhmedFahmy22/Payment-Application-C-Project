#include <stdio.h>
#include "../STD_Types.h"
#include "../Card/card.h"
#include "../Terminal/terminal.h"
#include "server.h"


ST_transaction astrucTransaction[255];

ST_accountsDB astrucDataBase[255]={
  { 50,     "19786786458456606", ALLOWED },
  { 500,    "04959346433356955", ALLOWED },
  { 5000,   "49508750123309905", ALLOWED },
  { 50000,  "15741437510083968", BLOCKED },
  { 500000, "55037007939576869", BLOCKED }
};


ST_transaction strInitTrans={
  {"000000000000000000000000","0000000000000000000","00/00"},
  {0,0,"00/00/0000"},
  APPROVED
};


EN_transState recieveTransactionData(ST_transaction *transData)
{
  uint8 u8CntrLoc,u8AccountIndxLoc;
  EN_transState enuErrorStateLoc = APPROVED;
  transData->transState = enuErrorStateLoc;
  if(transData==NULL_PTR)
  {
    enuErrorStateLoc = INTERNAL_SERVER_ERROR;
  }
  else
  {
    if(isValidAccount(&(transData->cardHolderData),&u8AccountIndxLoc)==OK_SERV)
    {
      if(isAmountAvailable(&(transData->terminalData),&astrucDataBase[u8AccountIndxLoc])!=OK_SERV)
      { 
		enuErrorStateLoc = DECLINED_INSUFFECIENT_FUND;
        printf("\nTransaction Canceled: Insuffecient Fund\n");
      }
    }
    else
    {
      enuErrorStateLoc = DECLINED_STOLEN_CARD;
      printf("\nTransaction Canceled: Stolen Card\n");
    }
  }
  transData->transState = enuErrorStateLoc;
  
  if(saveTransaction(transData)!=OK_SERV)
  {
    enuErrorStateLoc = INTERNAL_SERVER_ERROR;
    printf("\nTransaction Canceled: Internal Server Error\n");
  }
  if(enuErrorStateLoc==APPROVED)
  {
    astrucDataBase[u8AccountIndxLoc].balance=astrucDataBase[u8AccountIndxLoc].balance-transData->terminalData.transAmount;
    printf("\nTransaction Accepted\n");
  }
  return enuErrorStateLoc;
}

EN_serverError isValidAccount(ST_cardData *cardData,uint8 *u8AccountIndx)
{
  uint8 u8CntrLoc,u8CntrLoc2,u8Flag;
  EN_serverError enuErrorStateLoc = OK_SERV;
  for(u8CntrLoc=0;u8CntrLoc<255;u8CntrLoc++)
  {
    for(u8CntrLoc2=0,u8Flag=1;cardData->primaryAccountNumber[u8CntrLoc2]!='\0';u8CntrLoc2++)
    {
      if(cardData->primaryAccountNumber[u8CntrLoc2] != astrucDataBase[u8CntrLoc].primaryAccountNumber[u8CntrLoc2])
      {
        u8Flag=0;
        break;
      }
    }
    if(u8Flag==1)
    {
      *u8AccountIndx = u8CntrLoc;
      break;
    }
  }
  if(u8CntrLoc==255)
  {
    enuErrorStateLoc = DECLINED_STOLEN_CARD;
  }
  return enuErrorStateLoc;
}

EN_serverError isAmountAvailable(ST_terminalData *termData, ST_accountsDB *accountData)
{
  EN_serverError enuErrorStateLoc = OK_SERV;
  if((termData->transAmount)>(accountData->balance))
  {
    enuErrorStateLoc = LOW_BALANCE;
  }
  return enuErrorStateLoc;
}

EN_serverError saveTransaction(ST_transaction *transData)
{
  uint8 u8CntrLoc,u8CntrLoc2,u8Flag;
  EN_serverError enuErrorStateLoc = OK_SERV;

  for(u8CntrLoc=0;u8CntrLoc<255;u8CntrLoc++)
  {
    u8CntrLoc2=0,u8Flag=1;
    while(astrucTransaction[u8CntrLoc].cardHolderData.primaryAccountNumber[u8CntrLoc2]!='\0')
    {
      if(astrucTransaction[u8CntrLoc].cardHolderData.primaryAccountNumber[u8CntrLoc2]!='0')
      {
        u8Flag=0;
        break;
      }
      u8CntrLoc2++;
    }
    if(u8Flag==1)
    {
      break;
    }
  }
  if(u8CntrLoc==255||transData==NULL_PTR)
  {
    enuErrorStateLoc = SAVING_FAILED;
  }
  else
  {
    astrucTransaction[u8CntrLoc]=*transData;
    if(u8CntrLoc==0)
    {
      astrucTransaction[u8CntrLoc].transactionSequenceNumber=1;
    }
    else
    {
      astrucTransaction[u8CntrLoc].transactionSequenceNumber=astrucTransaction[u8CntrLoc-1].transactionSequenceNumber+1;
    }
  }
  return enuErrorStateLoc;
}

EN_serverError getTransaction(uint32 transactionSequenceNumber, ST_transaction *transData)
{
  uint8 u8CntrLoc;
  EN_serverError enuErrorStateLoc = OK_SERV;
  if(transData!=NULL_PTR)
  {
    for(u8CntrLoc=0;u8CntrLoc<255;u8CntrLoc++)
    {
      if(astrucTransaction[u8CntrLoc].transactionSequenceNumber==transactionSequenceNumber)
      {
        break;
      }
    }
    if(u8CntrLoc==255)
    {
      enuErrorStateLoc = TRANSACTION_NOT_FOUND;
    }
    else
    {
      *transData = astrucTransaction[u8CntrLoc];
    }
  }
  else
  {
    enuErrorStateLoc = NULL_POINTER_SERV;
  }
  return  enuErrorStateLoc;
}

void transactionArrayInit(void)
{
  uint8 u8CntrLoc;
  for(u8CntrLoc=0;u8CntrLoc<255;u8CntrLoc++)
  {
    astrucTransaction[u8CntrLoc]=strInitTrans;
  }
}

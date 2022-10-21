#include "Application/app.h"

int main(void)
{
  appStart();

  return 0;
}








































/*
void printTransData(ST_transaction *strTransData)
{
	printf("\nName: %s,	PAN: %s,	Expiry Date: %s\n",strTransData->cardHolderData.cardHolderName,strTransData->cardHolderData.primaryAccountNumber,strTransData->cardHolderData.cardExpirationDate);
	printf("Transaction Amount: %0.2f,	Max Trans. Amount: %0.2f,	Trans. Date: %s\n",strTransData->terminalData.transAmount,strTransData->terminalData.maxTransAmount,strTransData->terminalData.transactionDate);
	printf("Sequence Number: %d\n",strTransData->transactionSequenceNumber);
}*/

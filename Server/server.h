#ifndef SERVER_H
#define SERVER_H

typedef enum {
  APPROVED,
  DECLINED_INSUFFECIENT_FUND,
  DECLINED_STOLEN_CARD,
  INTERNAL_SERVER_ERROR
}EN_transState;

typedef enum {
  ALLOWED,
  BLOCKED
}EN_accountState;


typedef enum {
  OK_SERV,
  SAVING_FAILED,
  TRANSACTION_NOT_FOUND,
  ACCOUNT_NOT_FOUND,
  LOW_BALANCE,
  NULL_POINTER_SERV
}EN_serverError;

typedef struct {
  ST_cardData cardHolderData;
  ST_terminalData terminalData;
  EN_transState transState;
  uint32 transactionSequenceNumber;
}ST_transaction;

typedef struct {
  float32 balance;
  uint8 primaryAccountNumber[20];
  EN_accountState accountState;
}ST_accountsDB;

EN_serverError isValidAccount(ST_cardData *cardData,uint8 *u8AccountIndx);
EN_transState recieveTransactionData(ST_transaction *transData);
EN_serverError isAmountAvailable(ST_terminalData *termData, ST_accountsDB *accountData);
EN_serverError saveTransaction(ST_transaction *transData);
EN_serverError getTransaction(uint32 transactionSequenceNumber, ST_transaction *transData);
void transactionArrayInit(void);

extern ST_accountsDB astrucDataBase[255];
extern ST_transaction astrucTransaction[255];


#endif

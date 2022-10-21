#ifndef CARD_H
#define CARD_H

typedef struct {
  uint8 cardHolderName[25];
  uint8 primaryAccountNumber[20];
  uint8 cardExpirationDate[6];
}ST_cardData;

typedef enum {
  OK_CARD,
  WRONG_NAME,
  WRONG_EXP_DATE,
  WRONG_PAN,
  NULL_POINTER_CARD
}EN_cardError;

EN_cardError getCardHolderName(ST_cardData *cardData);
EN_cardError getCardExpiryDate(ST_cardData *cardData);
EN_cardError getCardPAN(ST_cardData *cardData);

#endif

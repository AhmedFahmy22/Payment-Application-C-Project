#ifndef TERMINAL_H
#define TERMINAL_H

typedef struct {
  float32 transAmount;
  float32 maxTransAmount;
  uint8 transactionDate[11];
}ST_terminalData;

typedef enum {
  OK_TERM,
  WRONG_DATE,
  EXPIRED_CARD,
  INVALID_CARD,
  INVALID_AMOUNT,
  EXCEED_MAX_AMOUNT,
  INVALID_MAX_AMOUNT,
  NULL_POINTER_TERM
}EN_terminal_Error;

EN_terminal_Error getTransactionDate(ST_terminalData *termData);
EN_terminal_Error isCardExpired(ST_cardData cardData, ST_terminalData termData);
EN_terminal_Error isValidCardPan(ST_cardData *cardData);
EN_terminal_Error getTransactionAmount(ST_terminalData *termData);
EN_terminal_Error isBelowMaxAmount(ST_terminalData *termData);
EN_terminal_Error setMaxAmount(ST_terminalData *termData);

#endif

/* -*- C++ -*- */

/****************************************************************************
** Copyright (c) quickfixengine.org  All rights reserved.
**
** This file is part of the QuickFIX FIX Engine
**
** This file may be distributed under the terms of the quickfixengine.org
** license as defined by quickfixengine.org and appearing in the file
** LICENSE included in the packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.quickfixengine.org/LICENSE for licensing information.
**
** Contact ask@quickfixengine.org if any conditions of this licensing are
** not clear to you.
**
****************************************************************************/

#ifndef TRADECLIENT_APPLICATION_H
#define TRADECLIENT_APPLICATION_H

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Values.h>
#include <quickfix/Mutex.h>

#include <quickfix/fix50/NewOrderSingle.h>
#include <quickfix/fix50/ExecutionReport.h>
#include <quickfix/fix50/OrderCancelRequest.h>
#include <quickfix/fix50/OrderCancelReject.h>
#include <quickfix/fix50/OrderCancelReplaceRequest.h>
#include <quickfix/fix50/MarketDataRequest.h>

#include <queue>

class Application :
      public FIX::Application,
      public FIX::MessageCracker
{
public:
  void run();

private:
  void onCreate( const FIX::SessionID& ) {}
  void onLogon( const FIX::SessionID& sessionID );
  void onLogout( const FIX::SessionID& sessionID );
  void toAdmin( FIX::Message&, const FIX::SessionID& ) {}
  void toApp( FIX::Message&, const FIX::SessionID& )
  throw( FIX::DoNotSend );
  void fromAdmin( const FIX::Message&, const FIX::SessionID& )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon ) {}
  void fromApp( const FIX::Message& message, const FIX::SessionID& sessionID )
  throw( FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType );

  void onMessage( const FIX50::ExecutionReport&, const FIX::SessionID& );
  void onMessage( const FIX50::OrderCancelReject&, const FIX::SessionID& );

  void queryEnterOrder();
  void queryCancelOrder();
  void queryReplaceOrder();
  void queryMarketDataRequest();

  FIX50::NewOrderSingle queryNewOrderSingle50();
  FIX50::OrderCancelRequest queryOrderCancelRequest50();
  FIX50::OrderCancelReplaceRequest queryCancelReplaceRequest50();
  FIX50::MarketDataRequest queryMarketDataRequest50();

  void queryHeader( FIX::Header& header );
  char queryAction();
  int queryVersion();
  bool queryConfirm( const std::string& query );

  FIX::SenderCompID querySenderCompID();
  FIX::TargetCompID queryTargetCompID();
  FIX::TargetSubID queryTargetSubID();
  FIX::ClOrdID queryClOrdID();
  FIX::OrigClOrdID queryOrigClOrdID();
  FIX::Symbol querySymbol();
  FIX::Side querySide();
  FIX::OrderQty queryOrderQty();
  FIX::OrdType queryOrdType();
  FIX::Price queryPrice();
  FIX::StopPx queryStopPx();
  FIX::TimeInForce queryTimeInForce();
};

#endif

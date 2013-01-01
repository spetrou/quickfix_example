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

#ifndef ORDERMATCH_APPLICATION_H
#define ORDERMATCH_APPLICATION_H

#include "IDGenerator.h"
#include "OrderMatcher.h"
#include "Order.h"
#include <queue>
#include <iostream>

#include <quickfix/Application.h>
#include <quickfix/MessageCracker.h>
#include <quickfix/Values.h>
#include <quickfix/Utility.h>
#include <quickfix/Mutex.h>

#include <quickfix/fix50/NewOrderSingle.h>
#include <quickfix/fix50/OrderCancelRequest.h>
#include <quickfix/fix50/MarketDataRequest.h>

class Application : public FIX::Application,
                    public FIX::MessageCracker
{
    /*!
     *  Overload for class 'FIX::Application'.
     */

    /*!
     *  @brief      Callback for Session create.
     *  @details    Callback function when the FIX Session gets
     *              created.
     *  @param  param1  The FIX Session ID.
     */
    void onCreate(const FIX::SessionID&)
    { }

    /*!
     *  @brief      Callback for Logon.
     *  @details    Callback function when the FIX Session receives
     *              a FIX Logon message.
     *  @param  param1  The FIX Session ID.
     */
    void onLogon(const FIX::SessionID& sessionID);

    /*!
     *  @brief      Callback for Logout.
     *  @details    Callback function when the FIX Session receives
     *              a FIX Logout message.
     *  @param  param1  The FIX Session ID.
     */
    void onLogout(const FIX::SessionID& sessionID);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void toAdmin(FIX::Message&,
                 const FIX::SessionID&)
    { }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void toApp(FIX::Message&,
               const FIX::SessionID&)
               throw(FIX::DoNotSend)
    { }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void fromAdmin(const FIX::Message&,
                   const FIX::SessionID&)
                   throw(FIX::FieldNotFound,
                         FIX::IncorrectDataFormat,
                         FIX::IncorrectTagValue,
                         FIX::RejectLogon)
    { }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void fromApp(const FIX::Message& message,
                 const FIX::SessionID& sessionID)
                 throw(FIX::FieldNotFound,
                       FIX::IncorrectDataFormat,
                       FIX::IncorrectTagValue,
                       FIX::UnsupportedMessageType);

    /*!
     *  Overload for class 'FIX::MessageCracker'.
     */

    /*!
     *  @brief      Callback for NewOrderSingle.
     *  @details    Callback function when a NewOrderSingle message is
     *              received. It extracts all the necessary fields from
     *              the incoming message in order to fully initialize an
     *              'Order' object and then it call a function to
     *              process it.
     *  @param  param1  The incoming NewOrderSingle object.
     *  @param  param2  The FIX Session ID.
     */
    void onMessage(const FIX50::NewOrderSingle&,
                   const FIX::SessionID&);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void onMessage(const FIX50::OrderCancelRequest&,
                   const FIX::SessionID&);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void onMessage(const FIX50::MarketDataRequest&,
                   const FIX::SessionID&);

    // Order functionality

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void processOrder(const Order&);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void processCancel(const std::string& id,
                       const std::string& symbol,
                       Order::Side);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void updateOrder(const Order&, char status);

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void rejectOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_REJECTED);
    }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void acceptOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_NEW);
    }
    
    /*!
     *  @brief
     *  @details
     *  @param
     */
    void fillOrder(const Order& order)
    {
        updateOrder(order,
                    order.isFilled() ? FIX::OrdStatus_FILLED
                                     : FIX::OrdStatus_PARTIALLY_FILLED);
    }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void cancelOrder(const Order& order)
    {
        updateOrder(order,
                    FIX::OrdStatus_CANCELED);
    }

    /*!
     *  @brief
     *  @details
     *  @param
     */
    void rejectOrder(const FIX::SenderCompID&,
                     const FIX::TargetCompID&,
                     const FIX::ClOrdID& clOrdID,
                     const FIX::Symbol& symbol,
                     const FIX::Side& side,
                     const std::string& message);

    // Type conversions

    /*!
     *  @brief      Convert FIX::Side enumeration.
     *  @details    Convert FIX::Side enumeration to the internal
     *              Order::Side equivalent.
     *  @param  param1  The FIX::Side to convert.
     *  @return The Order::Side enumeration.
     */
    Order::Side convert(const FIX::Side&) const;

    /*!
     *  @brief      Convert FIX::OrdType enumeration.
     *  @details    Convert FIX::OrdType enumeration to the internal
     *              Order::Type equivalent.
     *  @param  param1  The FIX::OrdType to convert.
     *  @return The Order::Type enumeration.
     */
    Order::Type convert(const FIX::OrdType&) const;

    /*!
     *  @brief      Convert Order::Side enumeration.
     *  @details    Convert Order::Side enumeration to the FIX
     *              FIX::Type equivalent.
     *  @param  param1  The Order::Side to convert.
     *  @return The FIX::Side enumeration.
     */
    FIX::Side convert(Order::Side) const;

    /*!
     *  @brief      Convert Order::Type enumeration.
     *  @details    Convert Order::Type enumeration to the FIX
     *              FIX::OrdType equivalent.
     *  @param  param1  The Order::Type to convert.
     *  @return The FIX::OrdType enumeration.
     */
    FIX::OrdType convert(Order::Type) const;

    OrderMatcher m_orderMatcher;
    IDGenerator m_generator;

public:

    /*!
     *  @brief
     *  @details
     *  @param
     */
    const OrderMatcher& orderMatcher() const
    {
        return m_orderMatcher;
    }
};

#endif

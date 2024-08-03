/*
*************************************************************
*                                                           *
*              Part of the Tracks Simulator                 *
*                                                           *
* Copyright: Marcelo Varanda                                *
* License: GPL3                                             *
*                                                           *
************************************************************
*/

#pragma once

#define ENABLE_LOGS

#ifdef ENABLE_LOGS
    #include <QDebug>
    #define LOG qDebug() << "Debug: "
    #define LOG_W qDebug() << "Warning: "
    #define LOG_E qDebug() << "Error: "
#else
    // TODO: create a dummy stream
    #define LOG qDebug() << "Debug: "
    #define LOG_W qDebug() << "Warning: "
    #define LOG_E qDebug() << "Error: "
#endif
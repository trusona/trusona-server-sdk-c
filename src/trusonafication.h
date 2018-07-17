/*
 * MIT License
 *
 * Copyright (c) 2016, 2017, 2018 Trusona Inc (www.trusona.com) All Rights Reserved
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef TRUSONAFICATION_H
#define TRUSONAFICATION_H

#include "trusona.h"

enum INPUT_TYPE {
  EMAIL_ADDRESS,
  TRUSONA_ID,
  USER_IDENTIFIER,
  INVALID_TYPE
};

enum TRUSONA_SDK_RESULT {
  TRUSONA_SUCCESS,
  TRUSONA_SERVICE_ERR,
  TRUSONA_INSUFFICIENT,
  TRUSONA_FAILURE,
  TRUSONA_INIT_ERR
};

const enum TRUSONA_SDK_RESULT trusonafy(TrusonaSession trusona_session);

#endif

/* TRUSONAFICATION_H */

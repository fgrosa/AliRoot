//-*- Mode: C++ -*-
// $Id$

#ifndef ALIHLTALTROGENERATOR_H
#define ALIHLTALTROGENERATOR_H
//* This file is property of and copyright by the ALICE HLT Project        * 
//* ALICE Experiment at CERN, All rights reserved.                         *
//* See cxx source for full Copyright notice                               *

/** @file   AliHLTAltroGenerator.h
    @author Matthias Richter
    @date   
    @brief  Simulation class of 10/40bit Altro Data.
*/

// see below for class documentation
// or
// refer to README to build package
// or
// visit http://web.ift.uib.no/~kjeks/doc/alice-hlt   

#include "AliHLTDataTypes.h"
#include "AliHLTLogging.h"
#include <vector>
#include <ostream>

class AliRawDataHeader;
class TArrayS;
class TArrayC;
class TRandom;

/**
 * @class AliHLTAltroGenerator
 */
class AliHLTAltroGenerator : AliHLTLogging {
 public:
  /** constructor */
  AliHLTAltroGenerator(int maxChannels=1000,
		       int maxBunches=50,
		       int maxBunchLength=10,
		       int maxTimebin=1024,
		       int maxSignal=500);
  /** destructor */
  virtual ~AliHLTAltroGenerator();

  /**
   * Generate a new event.
   * Simulate new data and store internally in plain format.
   *
   * @return size of the encoded data in byte. If the CDH and/or RCU
   * trailer was set, the size includes those.
   */
  int Generate();

  /**
   * Get the number of 40bit Altro words of the current data set.
   * @return number of 40bit Altro words.
   */
  int GetNof40BitAltroWords() const;

  /**
   * Get the data size of the current data set.
   * @return size of the encoded data in byte. If the CDH and/or RCU
   * trailer was set, the size includes those.
   */
  int GetDataSize();

  /**
   * Get the simulated data.
   * Get a pointer to the internal buffer. The buffer is filled with
   * the encoded data from the previous simulation.
   * @param pBuffer     target variable to receive the pointer
   * @return size in byte, neg. error if failed
   */
  int GetData(AliHLTUInt8_t* &pBuffer);

  /**
   * Get the simulated data.
   * The provided buffer is filled with the encoded data from the
   * previous simulation.
   * @param pBuffer     target variable to receive the pointer
   * @return size in byte, neg. error if failed
   */
  int GetData(AliHLTUInt8_t* pBuffer, int size);

  /**
   * Set the Common Data Header.
   * @param pCDH        the CDH
   * @param size        size of the header in byte
   * @return neg. error code if failed
   */
  int SetCDH(AliRawDataHeader* pCDH, int size);

  /**
   * Set the RCU trailer.
   * @param pTrailer    the trailer
   * @param size        size of the header in byte
   * @return neg. error code if failed
   */
  int SetRCUTrailer(AliHLTUInt8_t* pTrailer, int size);

  /**
   * Get list of channels in the current data set.
   */
  int GetChannels(vector<AliHLTUInt16_t> list);

  /**
   * Set array of channels for sorting of channels.
   * The encoded data will be sorted according to the specified
   * list.
   * @param array       array of channels
   * @param arraySize   size of the array
   */
  int SetSorting(AliHLTUInt16_t *array, int arraySize);

  /**
   * Get a random number in the given range.
   */
  int GetRandom(int min, int max);

  /**
   * Set parsing direction for the Next functions.
   * @param direction   @ref AliHLTAltroGenerator::kBackwards (default),
   *                    @ref AliHLTAltroGenerator::kForwards
   */
  void SetDirection(int direction) {fDirection=direction;}

  /**
   * Position at the next signal.
   * The function follows the pure stream model.
   * @return true if there is a new signal available
   */
  bool Next();

  /**
   * Get the current signal.
   * The current time value can be retrieved by ::GetStartTime or
   * ::GetEndTime which return both the current time in the stream
   * model.
   * @return signal value
   */
  AliHLTUInt16_t GetSignal();

  /**
   * Position at the beginning of the next channel.
   * Depending on the mode, the function works either back or
   * forwards.
   * @return true if there is a new channel available
   */
  bool NextChannel();

  /**
   * Get the hardware address of the current channel.
   */
  AliHLTUInt16_t GetHwAddress();

  /**
   * Get bunch count of the current channel
   */
  int GetBunchCount();

  /**
   * Position at the beginning of the next bunch.
   * Depending on the mode, the function works either back or
   * forwards.
   * @return true if there is a new bunch available
   */
  bool NextBunch();

  /**
   * Get size of the current bunch.
   */
  AliHLTUInt16_t GetBunchSize();

  /**
   * Get start time of the current bunch or signal.
   */
  AliHLTUInt16_t  GetStartTime();

  /**
   * Get end time of the current bunch or signal.
   */
  AliHLTUInt16_t  GetEndTime();

  /**
   * Get pointer to signals of current bunch.
   * The signals are always in ascending order.
   */
  const Short_t* GetSignals();

  /**
   * Reset the internal position variables.
   */
  int Reset();

  /**
   * Rewind stream position for Next funxtions
   */
  int Rewind();

  /**
   * Print content of simulated data to cout.
   */
  void Print();

  /**
   * Printout of simulated data.
   */
  friend ostream &operator<<(ostream &str, AliHLTAltroGenerator &generator);

  enum {
    kBackwards = 0,
    kForwards = 1
  };

 protected:
 
 private:
  /** copy constructor prohibited */
  AliHLTAltroGenerator(const AliHLTAltroGenerator&);
  /** assignment operator prohibited */
  AliHLTAltroGenerator& operator=(const AliHLTAltroGenerator&);

  /**
   * Encode the simulated data into Altro format
   */
  int EncodeData(AliHLTUInt8_t* pBuffer, int size);

  /// internal data buffer
  TArrayC* fpData; //!transient

  /// array of simulated data
  TArrayS* fpSimData; //! transient

  struct AliChannelPosition {
    AliHLTUInt16_t fChannel; //! transient
    int fPosition; //! transient
    int fEnd; //! transient
  };

  /// channels and their positions in the simulated data
  vector<AliChannelPosition> fChannelPositions; //! transient

  /// the Altro payload in the simulated data
  int fNof10BitWords; //! transient

  /// the Common Data Header
  AliRawDataHeader* fpCDH; //!transient

  /// size of the Common Data Header in byte
  int fCDHSize; //! transient

  /// the RCU trailer
  AliHLTUInt8_t* fpTrailer; //!transient

  /// size of the trailer
  int fTrailerSize; //!transient

  /// maximum number of channels
  int fMaxChannels; //! transient

  /// maximum number of bunches
  int fMaxBunches; //! transient

  /// maximum bunche length
  int fMaxBunchLength; //! transient

  /// max timebin
  int fMaxTimebin; //!transient

  /// maximum signal
  int fMaxSignal; // transient

  /// the random number generator
  TRandom* fpRand; //! transient

  /// direction of parsing
  int fDirection; //! transient

  /// current channel position for the Next functions
  int fCurrentPosition; //! transient

  /// current bunch position in the simulated data
  int fCurrentBunch; //! transient

  /// current offset in the current bunch
  int fCurrentTimeOffset; //! transient

  ClassDef(AliHLTAltroGenerator, 0);
};

#endif

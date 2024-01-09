#pragma once
/*****************************************************************//**
 * @file   FrameRate.h
 * @brief  FPS制御を行うヘッダー
 * 
 * @author Wrelf
 *********************************************************************/
#include "Essentials.h"
#include <chrono>
#include <Util.h>
/**
 * @brief FPS制御を行うクラス
 */
class FrameRate
{
public:
	/**
	 * @brief 各フレームの開始を待つ
	 * 
	 */
	static void FrameStartWithWait();

	/**
	 * @brief フレーム開始時のマークをリセット
	 * 
	 */
	static void InitMark();

private:
	static std::chrono::system_clock::time_point  sFrameend, sFramestart;

	static class WaitTimeData {
	public:
		int32_t waitTimeBase_ = 16666;

		template <class Archive>
		void serialize(Archive& ar) {
			ar(waitTimeBase_);
		}

	} sWaitTimeData;
};


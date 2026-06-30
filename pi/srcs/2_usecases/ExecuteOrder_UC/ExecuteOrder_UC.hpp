#pragma once
#include "1_domain/System/System.hpp"
#include "2_usecases/CaptureData_UC/CaptureData_UC.hpp"
#include "2_usecases/SendPhotoToComputer_UC/SendPhotoToComputer_UC.hpp"
#include "2_usecases/SendToComputer_UC/SendToComputer_UC.hpp"
#include "3_interface/IPico.hpp"

class ExecuteOrder_UC
{
private:
    CaptureData_UC    &_capture;
    SendPhotoToComputer_UC       &_send;
    SendToComputer_UC &_sender;
    IPico       &_pico;

    int  _dispatch(const std::string &cmd);
    int  _handleTakePhoto();
    int  _handlePingPico();
    int  _handleGetPicoStatus();
	int	 _handleTestHardware();
    int  _handlePlateNext();
    int  _handlePosition(const std::string &armPose, const std::string &camPose);
    int  _handlePositionA();
    int  _handlePositionB();
    int  _handlePositionC();
    int  _handlePositionD();
    int  _handleInitialPos();
    bool _isNoOp(const std::string &cmd) const;

public:
    ExecuteOrder_UC(CaptureData_UC &capture, SendPhotoToComputer_UC &send,
                    SendToComputer_UC &sender, IPico &pico);
    ~ExecuteOrder_UC() = default;

    void execute(System &sys);
};

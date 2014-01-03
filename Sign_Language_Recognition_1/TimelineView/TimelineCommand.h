/* by ywpg */
/* Last Modified: 2007/11/12 05:57:22 . */

#ifndef _TIMELINE_COMMAND_H_INCLUDED_
#define _TIMELINE_COMMAND_H_INCLUDED_

#pragma warning(disable:4819)

#include "../../Command.h"
#include "../../DataModel/PulsarModel.h"

namespace Pulsar
{
	class PlayMotion
		: public NotUndoableCommand
	{
		bool _isForward;

	public:
		typedef void result_type;

		PlayMotion(bool isForward)
			:_isForward(isForward)
		{
		}

		~PlayMotion()
		{
		}

	protected:
		boost::any exec(void)
		{
			if (_isForward)
			{
				PulsarModel::instance().set_timeline_nPlayDirection(1);
			}
			else
			{
				PulsarModel::instance().set_timeline_nPlayDirection(-1);
			}

			PulsarModel::instance().enable_play_timer(true);

			return boost::any(); // null
		}

	public:
		virtual const QString GetCommandName(void) const
		{
			return "PlayMotion";
		}

		virtual const QString GetCommandDescription(void) const
		{
			return "Plays current motion.";
		}


	}; 


	class EnablePlayMotionTimer
		: public NotUndoableCommand
	{
		bool _play;

	public:
		typedef void result_type;

		EnablePlayMotionTimer(bool play)
			:_play(play)
		{
		}

		~EnablePlayMotionTimer()
		{
		}
	
	protected:
		boost::any exec(void)
		{
			if (_play)
			{
				PulsarModel::instance().enable_play_timer(true);
			}
			else
			{
				PulsarModel::instance().enable_play_timer(false);
			}

			return boost::any(); // null
		}

	public:
		virtual const QString GetCommandName(void) const
		{
			return "EnablePlayMotionTimer";
		}

		virtual const QString GetCommandDescription(void) const
		{
			return "Toggle timer on/off for play/stop motion.";
		}

	};

	class PlayMotion_Step
		: public NotUndoableCommand
	{
		int direction;
		public:
		typedef void result_type;

		PlayMotion_Step(int _direction)
			: direction(_direction)
		{
		}
		~PlayMotion_Step()
		{
		}
		protected:
		boost::any exec(void)
		{
			PulsarModel::instance().timeline_play_take_a_step(direction);

			return boost::any();
		}
		public:
		virtual const QString GetCommandName(void) const
		{
			return "Play motion of one frame";
		}
		virtual const QString GetCommandDescription(void) const
		{
			return "play motion for one step.";
		}
	};

	class SetCurrentFrameNo
		: public NotUndoableCommand
	{
		int n;
	public:
		typedef void result_type;

		SetCurrentFrameNo(int _n)
			: n(_n)
		{
		}
		~SetCurrentFrameNo()
		{
		}
	protected:
		boost::any exec(void)
		{
			PulsarModel::instance().SetCurrentFrame(n);

			return boost::any();
		}
	public:
		virtual const QString GetCommandName(void) const
		{
			return "SetCurrentFrameNumber";
		}
		virtual const QString GetCommandDescription(void) const
		{
			return "Set current frame number.";
		}
	};

} // end of namespace Pulsar

#endif //_TIMELINE_COMMAND_H_INCLUDED_

/* vim: set ft=cpp: */

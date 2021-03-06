///
/// Copyright (c) 2009-2015 Nous Xiong (348944179 at qq dot com)
///
/// Distributed under the Boost Software License, Version 1.0. (See accompanying
/// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///
/// See https://github.com/nousxiong/gce for latest version.
///

namespace gce
{
namespace asio
{
class signal_ut
{
public:
  static void run()
  {
    std::cout << "signal_ut begin." << std::endl;
    for (std::size_t i=0; i<test_count; ++i)
    {
      test_base();
      if (test_count > 1) std::cout << "\r" << i;
    }
    if (test_count > 1) std::cout << std::endl;
    std::cout << "signal_ut end." << std::endl;
  }

private:
  static void my_actor(stackful_actor self)
  {
    log::logger_t& lg = self.get_context().get_logger();
    try
    {
      signal sig(self, SIGINT, SIGTERM);
      aid_t sender = self->match("init").recv();
      while (true)
      {
        message msg;
        match_t type;
        self->match("echo", "quit", type).raw(msg).recv();
        if (type == atom("quit"))
        {
          break;
        }

        sig.async_wait();
        sig->cancel();
        errcode_t ec;
        self->match(as_signal).recv(ec);
        GCE_VERIFY(ec);

        self.send(sender, msg);
      }

      /// test quit
      sig.async_wait();
    }
    catch (std::exception& ex)
    {
      GCE_ERROR(lg) << ex.what();
    }
  }

  static void test_base()
  {
    log::asio_logger lgr;
    log::logger_t lg = boost::bind(&gce::log::asio_logger::output, &lgr, _arg1, "");

    try
    {
      size_t size = 10;
      attributes attrs;
      attrs.lg_ = lg;
      context ctx(attrs);
      threaded_actor base = spawn(ctx);
      signal sig(base, SIGINT, SIGTERM);
      std::vector<aid_t> echo_list(size);

      for (size_t i=0; i<size; ++i)
      {
        aid_t aid = spawn(base, boost::bind(&signal_ut::my_actor, _arg1), monitored);
        base->send(aid, "init");
        echo_list[i] = aid;
      }

      for (size_t n=0; n<10; ++n)
      {
        for (size_t i=0; i<size; ++i)
        {
          base->send(echo_list[i], "echo", "hi");
        }

        for (size_t i=0; i<size; ++i)
        {
          errcode_t ec;
          message msg;
          match_t type;
          std::string str;
          base->match("echo", type).guard(echo_list[i], ec).raw(msg).recv();
          GCE_VERIFY(!ec).except(ec);
          GCE_VERIFY(type == atom("echo"));

          msg >> str;
          GCE_VERIFY(str == "hi");

          sig.async_wait();
          sig->cancel();
          base->match(as_signal).recv(ec);
          GCE_VERIFY(ec);
        }
      }

      for (size_t i=0; i<size; ++i)
      {
        base->send(echo_list[i], "quit");
        base->recv(exit);
      }
    }
    catch (std::exception& ex)
    {
      GCE_ERROR(lg) << ex.what();
    }
  }
};
}
}

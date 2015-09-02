#pragma once
#include "typedef.hpp"
#include "config.hpp"
#include <mysql++.h>
#include <boost/thread.hpp>
#include <boost/thread/tss.hpp>

class dbreader
    :boost::noncopyable
    ,public gce::addon_t
{
    typedef gce::addon_t base_t;
public:
    template <typename Actor>
    dbreader(Actor& a)
        : addon_t(a)
    {
    }

    void run(gce::stackful_actor self, const config& cfg)
    {    
        cfg_ = cfg;
        boost::asio::io_service io_service;
        boost::thread_group threads;
        boost::asio::io_service::work wk(io_service);
        for (int i = 0; i < cfg_.db_thread_num; i++)
        {
            threads.create_thread(boost::bind(&dbreader::db_load_thread, this, &io_service, self.get_aid()));
        }
            
        std::shared_ptr<p::xs2ds_entity_req> req;
        while (true)
        {
            gce::aid_t sender = self->match(XS2DS_ENTITY_REQ).recv(req);
            // 发送加载db post
        }

    }

private:
    void send_back(std::string const& str)
    {
        gce::io_service_t& ios = base_t::get_strand().get_io_service();
        ios.post(boost::bind(&dbreader::pri_send_back, this, str));
    }

    void pri_send_back(std::string const& str)
    {
        gce::message m("echo");
        m << str;
        base_t::send2actor(m);
    }

    void db_load_thread(boost::asio::io_service& srv, gce::aid_t)
    {
        conn_.reset(new mysqlpp::Connection);
        conn_->set_option(new mysqlpp::SetCharsetNameOption("utf8"));
        if (!connect_to_db(*conn_))
        {
            printf("connect the database failed\n");
            return;
        }

        printf("sql client version is <%s>\n", conn_->client_version().c_str());

        boost::system::error_code error;
        while (true)
        {
            srv.run_one(error);
        }
    }

    bool connect_to_db(mysqlpp::Connection& conn)
    {
        return connect_to_db(conn, cfg_.db_ip, cfg_.db_port,
            cfg_.db_user, cfg_.db_pwd, cfg_.db_name);
    }

    bool connect_to_db(mysqlpp::Connection& conn, const std::string& host, uint32_t port,
        const std::string& user, const std::string& password, const std::string& database_name)
    {
        try
        {
            if (host.size() == 0 || user.size() == 0 || password.size() == 0)
                return false;

            if (conn.connected())
                return true;

            conn.connect(database_name.c_str(),
                host.c_str(),
                user.c_str(),
                password.c_str(),
                port);
            printf("Connect to db `%s@%s`\n", database_name.c_str(), host.c_str());
            return true;
        }
        catch (const ::std::exception& er)
        {
            printf("Connect mysql db failed: %s\n", er.what());
        }

        return false;
    }

private:
    config  cfg_;
    boost::thread_specific_ptr<mysqlpp::Connection> conn_;  // 线程本地存储
};


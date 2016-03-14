// Copyright (c) 2006, 2007 Julio M. Merino Vidal
// Copyright (c) 2008 Ilya Sokolov, Boris Schaeling
// Copyright (c) 2009 Boris Schaeling
// Copyright (c) 2010 Felipe Tanus, Boris Schaeling
// Copyright (c) 2011, 2012 Jeff Flinn, Boris Schaeling
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef HPX_PROCESS_POSIX_INITIALIZERS_SET_CMD_LINE_HPP
#define HPX_PROCESS_POSIX_INITIALIZERS_SET_CMD_LINE_HPP

#include <hpx/config.hpp>

#if !defined(HPX_WINDOWS)
#include <hpx/components/process/util/posix/initializers/initializer_base.hpp>
#include <boost/tokenizer.hpp>
#include <boost/shared_array.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <string>
#include <vector>

namespace hpx { namespace components { namespace process { namespace posix {

namespace initializers {

class set_cmd_line : public initializer_base
{
private:
    static char *c_str(const std::string &s)
    {
        return const_cast<char*>(s.c_str());
    }

public:
    explicit set_cmd_line(const std::string &s)
    {
        typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;
        boost::escaped_list_separator<char> sep('\\', ' ', '\"');
        tokenizer tok(s, sep);
        args_.assign(tok.begin(), tok.end());
        cmd_line_.reset(new char*[args_.size() + 1]);
        boost::transform(args_, cmd_line_.get(), c_str);
        cmd_line_[args_.size()] = 0;
    }

    template <class PosixExecutor>
    void on_exec_setup(PosixExecutor &e) const
    {
        e.cmd_line = cmd_line_.get();
    }

private:
    std::vector<std::string> args_;
    boost::shared_array<char*> cmd_line_;
};

}

}}}}

#endif
#endif